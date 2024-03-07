# expected config format

## header
the webserv config is to be prefixed by the keyword **`webserv`**, directly followed (excluding comments & whitespaces) by an opening brace **`{`**.
## comments
every character following **`#`** will be ignored in parsing until the next newline.
## scopes
### declare a scope
a new scope is to be opened/closed by **`{}`** and declared with a name, eg:

```yaml
/admin # this will used as the scope key
{ # opening brace
	root	/tmp; # value level
} # closing brace
```
failure to initialize a scope with a name will cause the configuration file to be considered invalid, eg:
```yaml
... 
} # opening new scope with no identifier
{
	root	/tmp;
}
```
this can be simplified to:

```cpp
if (line == "{" && prev_line.is_delimiter() == true)
{
	config = denied;
}	
```
### closing of scope
any unclosed scope can lead to values being stored incorrectly -> undefined behavior, thus:
```cpp
if (num_of_opening_braces != num_of_closing_braces)
{
	config = denied;
}
```
### value level:
a line followed (excluding comments) by **`;`** will be parsed as value level, eg:

`allowed_methods	POST GET;`

the line will be split into words (by whitespaces), the first word being considered the key & the following words the values.

thus, a line followed by **`;`** & containing less than 2 words will be considered invalid and ignored in parsing.
### duplicate keys within scope
duplicate keys within the same scope will be merged, eg:
```yaml
webserv
{
	port			80;
	server_name		domain.com;
	host			localhost;
	server_name		www.domain.com;
}
```
**-->** `config["webserv:server_name"] = "domain.com", "www.domain.com"`
### empty scopes
empty scopes, eg:
```
routes
{

}
```
will be ignored in parsing and have no effect on the configuration of the server.
## characters
#### escape characters
any escape like **`\t`**, **`\n`**, **`\\`** will be parsed as literals, eg:

`host	local\nhost;` will be read as `config["webserv:host"] = "localhost"`