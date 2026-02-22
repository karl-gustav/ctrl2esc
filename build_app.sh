#!/bin/bash
set -e

APP_NAME="Ctrl2Esc"
BIN_NAME="ctrl2esc"   # your compiled binary
ICON_NAME="icon.icns" # your icon file

if [ ! -f "$BIN_NAME" ]; then
    echo "$BIN_NAME not found. Build your C program first."
    exit 1
fi

echo "Creating $APP_NAME.app …"

# Build directories
mkdir -p "$APP_NAME.app/Contents/MacOS"
mkdir -p "$APP_NAME.app/Contents/Resources"

# Copy binary
cp "$BIN_NAME" "$APP_NAME.app/Contents/MacOS/$BIN_NAME"
chmod +x "$APP_NAME.app/Contents/MacOS/$BIN_NAME"

# Copy icon
cp "$ICON_NAME" "$APP_NAME.app/Contents/Resources/$ICON_NAME"

# Launcher stub (recommended)
cat > "$APP_NAME.app/Contents/MacOS/launcher" << EOF
#!/bin/bash
DIR="\$(cd "\$(dirname "\$0")" && pwd)"
exec "\$DIR/$BIN_NAME" 2>> "/tmp/ctrl2esc.log"
EOF
chmod +x "$APP_NAME.app/Contents/MacOS/launcher"

# Info.plist
cat > "$APP_NAME.app/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>CFBundleExecutable</key>
    <string>launcher</string>

    <key>CFBundleIdentifier</key>
    <string>com.github.karl-gustav.ctrl2esc</string>

    <key>CFBundleName</key>
    <string>Ctrl2Esc</string>

    <key>CFBundleVersion</key>
    <string>1.0</string>

    <key>CFBundlePackageType</key>
    <string>APPL</string>

    <key>LSBackgroundOnly</key>
    <true/>

    <key>LSUIElement</key>
    <true/>

    <key>CFBundleIconFile</key>
    <string>$ICON_NAME</string>

    <key>NSAccessibilityUsageDescription</key>
    <string>Ctrl2Esc requires accessibility permissions to capture keyboard events.</string>
  </dict>
</plist>
EOF

echo "Done! Install to Accessibility: $APP_NAME.app"
