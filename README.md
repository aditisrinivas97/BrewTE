# BrewTE

[![forthebadge](https://forthebadge.com/images/badges/made-with-c.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/pretty-risque.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)

A minimalistic terminal-based text editor.

# Installing 

For those of you who dont like reading instructions and prefer watching a video, click on the more than obvious link below!

[![asciicast](https://asciinema.org/a/YJbgYpy0alQTWEBTsN8FhVCmc.png)](https://asciinema.org/a/YJbgYpy0alQTWEBTsN8FhVCmc)

If you prefer reading, here are the instructions to install this betamax text editor!

Clone this repository

```
$ git clone https://github.com/aditisrinivas97/BrewTE
```

Add an alias to the editor bin present in the `BrewTE` folder. If you're using Bash on mac, in your terminal, execute the following command

```
$ echo "alias brewte="path/to/BrewTE/editor"" >> ~/.bash_profile && source ~/.bash_profile
```

Execute the `brewte` command to test if the text editor is up and running! You should see something like this

![SS](https://github.com/aditisrinivas97/BrewTE/blob/master/extras/ss.png)

# Usage

## Creating files

You can create and edit a file easily by running

```
$ brewte filename
```

To edit an existing file, do the same as above.

If no name is specified, upon saving, the file's name will be a `timestamp` of when it was saved. 

Watch the video below to see a demo of all the features supported by BrewTE!

[![asciicast](https://asciinema.org/a/cG5FGxsM37rKgGeAAxCJOl5F8.png)](https://asciinema.org/a/cG5FGxsM37rKgGeAAxCJOl5F8)

## License

This project is made available under the [MIT License](http://www.opensource.org/licenses/mit-license.php).