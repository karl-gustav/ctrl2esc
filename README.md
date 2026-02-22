# Ctrl2Esc

Tiny MacOS specific C program to send a ESC key press if I tap the Ctrl key.

This is useful in vim like programs that are ESC and Ctrl heavy programs. Another tip is to map the Caps Lock key to
Ctrl and it'll be even easier to use.

## Building

`make` or `make build`

## Installing

Build the app (see Building section above) and copy the resulting Ctrl2Esc.app to your /Applications/ folder. Run the
app, set `Settings > Privacy & Security > Accessibility` to true in MacOS settings when prompted by the app. Run the app
again (it doesn't start when the Accessibility dialog shows up. Add it to `Settings > Open at Login` in MacOS settings.

