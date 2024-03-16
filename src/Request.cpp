
#include "Request.hpp"


Request::~Request()
{
}

std::ostream& operator<<(std::ostream& os, const Request& req)
{
    // print request with name in red
    os << RED <<"Method: " << RESET << req.get_method() << std::endl;
    os << RED << "URI: " << RESET << req.get_uri() << std::endl;
    os << RED << "Protocol: " << RESET  << req.get_protocol() << std::endl;
    os << RED << "Headers: " << RESET << std::endl;
    std::map <std::string, std::string> headers = req.get_headers();
    for (std::map <std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    {
        os << "\t" << it->first << ": " << it->second << std::endl;
    }
    os << RED  << "Body: \n" << RESET;
    std::stringstream ss(req.get_body());
    std::string line;
    while (std::getline(ss, line)) {
        std::cout << "\t" << line << std::endl;
    }
    return os;
}

void Request::set_protocol(const std::string& val)
{
    this->protocol = val;
}

std::string Request::get_protocol() const
{
    return this->protocol;
}

std::string Request::get_method() const
{
    return this->method;
}

void Request::set_method(const std::string& val)
{
    this->method = val;
}

std::string Request::get_uri() const
{
    return this->uri;
}

void Request::set_uri(const std::string& val)
{
    this->uri = val;
}

std::map <std::string, std::string> Request::get_headers() const
{
    return this->headers;
}

void Request::set_headers(const std::map <std::string, std::string>& val)
{
    this->headers = val;
}

std::string Request::get_body() const
{
    return this->body;
}

void Request::set_body(const std::string& val)
{
    this->body = val;
}

std::string Request::get_header(const std::string& key) const
{
    if (this->headers.find(key) != this->headers.end())
    {
        return this->headers.at(key);
    }
    return NULL;
}
// parse request
//
// @param request: request string
void Request::parse(std::string request){
    std::string line;
    std::istringstream iss(request);
    std::getline(iss, line);
    //check that there are only 2 spaces in the line
    if (std::count(line.begin(), line.end(), ' ') != 2)
    {
        std::cout << line << std::endl;
        std::cout << std::count(line.begin(), line.end(), ' ') << std::endl;
        throw std::runtime_error("Invalid request: line does not contain 2 spaces between method, uri and protocol");
    }
    //check that there are now white spaces in the line except for the 2 spaces
    for (size_t i = 0; i < line.size() - 2; i++)
    {
        if (std::isspace(line[i]) != 0 && line[i] != ' ')
        {
            std::cout << i << std::endl;
            std::cout << (int)line[i] << std::endl;
            throw std::runtime_error("Invalid request: line contains white spaces except for the 2 spaces between method, uri and protocol");
        }
    }

    //check that line ends with \r\n
    if (line[line.size() - 1] != '\r')
    {
        throw std::runtime_error("Invalid request: line does not end with \\r");
    }
    std::istringstream iss_line(line);
    std::string method;
    std::string uri;
    std::string protocol;
    // get method, uri, protocol and check that they are not empty
    iss_line >> method;
    iss_line >> uri;
    iss_line >> protocol;
    if (method.size() == 0 || uri.size() == 0 || protocol.size() == 0)
    {
        throw std::runtime_error("Invalid request: method, uri or protocol is empty");
    }
    this->set_uri(method);
    this->set_method(uri);
    this->set_protocol(protocol);

    // get headers
    while (std::getline(iss, line))
    {
        if (line == "\r")
        {
            break;
        }
        std::istringstream iss_line(line);
        std::string key;
        std::string value;
        std::getline(iss_line, key, ':');
        std::getline(iss_line, value);
        if (value.end()[-1] != '\r')
        {
            throw std::runtime_error("Invalid request: header does not end with \\r");
        }
        //key to upper
        for (size_t i = 0; i < key.size(); i++)
        {
            key[i] = std::toupper(key[i]);
        }
        if (this->headers.find(key) != this->headers.end())
        {
            value = this->headers[key] + "," + Utils::trim(value, " \t\n\r");
            this->headers[key] = value;
        }
        else{
            this->headers[key] = Utils::trim(value, " \t\n\r");}
        
    }
    // get body
    while (std::getline(iss, line))
    {
        this->body += line + "\n";
    }

}
// go through headers and validate for printable ascii characters
    //token          = 1*tchar
    /* tchar          = "!" / "#" / "$" / "%" / "&" / "'" / "*"
                / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
                / DIGIT / ALPHA
                ; any VCHAR, except delimiters */
//
// @param str: string to validate
// @return: true if valid, false otherwise
static bool valid_token(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        int c = str[i];
        if (!(std::isalnum(c) || c == '!' || c == '#' || c == '$' || c == '%' ||
              c == '&' || c == '\'' || c == '*' || c == '+' || c == '-' ||
              c == '.' || c == '^' || c == '_' || c == '`' || c == '|' ||
              c == '~')) {
            return false;
        }
    }
    return true;
    }
// validate request

void Request::validate()
{
    if (this->get_method() != "GET" && this->get_method() != "POST" && this->get_method() != "DELETE")
    {
        throw std::runtime_error("Invalid method");
    }
    if (this->get_protocol() != "HTTP/1.1"){
        throw std::runtime_error("Invalid protocol");
    }
    // validate headers - field-names
    for (std::map <std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
    {
        if (it->first.size() == 0)
        {
            throw std::runtime_error("Invalid header: empty field-name");
        }
        if (!valid_token(it->first))
        {
            throw std::runtime_error("Invalid header: invalid field-name");
        }
    }
    // validate headers - field-values - printable ascii characters
    for (std::map <std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            if (!std::isprint(it->second[i]))
            {
                throw std::runtime_error("Invalid header: invalid field-value");
            }
        }
    }
    // validate uri

}
// constructor
Request::Request(std::string request)
{
    parse(request);
    validate();
    

}