# dwmblocks
Modular status bar for dwm written in c.
Scripts can be found in my [dotfiles](https://github.com/jedi2610/dotfiles/tree/master/.local/bin/dwmblocks) repo.


# usage
To use dwmblocks first run `make` and then install it with `doas make install`.


# modifying blocks
The statusbar is made from text output from commandline programs.
Blocks are added and removed by editing the `config.h` header file.


# signaling
Most statusbars constantly rerun every script every several seconds to update. This is an option here, but a superior choice is giving your module a signal that you can signal to it to update on a relevant event, rather than having it rerun idly.

For example, the volume module has the update signal 10 by default. Thus, running `pkill -RTMIN+10 dwmblocks` will update it.

You can also run `kill -44 $(pidof dwmblocks)` which will have the same effect, but is faster. Just add 34 to your typical signal number.

My volume module never updates on its own, instead I have this command run along side my volume shortcuts in dwm to only update it when relevant.

Note that all modules must have different signal numbers.


# patches
This build allows you to build in additional actions into your scripts in response to click events. 
See the above linked scripts for examples of this using the $BUTTON variable. The statuscmd patch can be found [here](https://dwm.suckless.org/patches/statuscmd):
