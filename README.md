# SSGifier

This is a CLI SSG(Static Site Generator) that generates static website.

# Optional functions implemented

- allow the user to specify a different output directory using --output or -o. If not specified, dist will be used, but if the user specifies a different output path, use that. Print an error if the specified output path is not a valid directory.
- allow the user to optionally specify a --stylesheet or -s URL to a CSS stylesheet to be used in the <head> of your generated HTML files.

# How to use

This is a CLI software which requires to use terminal to run it.

The list below is all the different parameters you can pass to SSGifier.  
-v --version - prints out the version of SSGifier.
-o, --output - specify the output directory, the directory " dlist" is used by default.(Optional)  
-i, --input - specify the input files\directory, this is an necessary argument.  
-s, --stylesheet - specify the stylesheet for the HTML pages.(Optional)  
-h, --help - show help.
-c, --config - config command

# Updated

Markdown Support added for # Headings and \* Italics just use a file with a .md file extension

JSON Support with the config option Using the JSON parser from this Repository [nlohmann/json](https://github.com/nlohmann/json)

More Markdown support for Markdown Headers, Code blocks, links, lists, and styles(bold, itatlic, quotes, etc.) by using [maddy](https://github.com/progsource/maddy)

# License

[Do What The Fuck You Want To Public License(WTFPL)](https://raw.githubusercontent.com/liutng/SSGifier/main/License.md)
