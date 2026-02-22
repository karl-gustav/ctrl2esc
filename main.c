#include <ApplicationServices/ApplicationServices.h>
#include <time.h>

enum { KEY_ESC = 53, KEY_LCTRL = 59, KEY_RCTRL = 62 };

static int down = 0, usedAsMod = 0, activeKey = 0;
static double downAt = 0.0;
static const double tapThreshold = 0.200;

static void postEscape(void) {
    CGEventRef d = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)KEY_ESC, true);
    CGEventRef u = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)KEY_ESC, false);

    CGEventSetFlags(d, 0);
    CGEventSetFlags(u, 0);

    CGEventPost(kCGHIDEventTap, d);
    CGEventPost(kCGHIDEventTap, u);

    if (d) CFRelease(d);
    if (u) CFRelease(u);
}

static int isModifier(int64_t code) {
    switch (code) {
        case 54: case 55: case 56: case 60: case 57: case 58: case 61: case 59: case 62: return 1;
        default: return 0;
    }
}

static char* timestamp() {
        time_t rawtime;
        struct tm *info;
        static char buffer[20];

        time(&rawtime);
        info = gmtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", info);
        return buffer;
}

static CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo) {
    if (type == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput) return event;
    int64_t kc = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    switch (type) {
        case kCGEventFlagsChanged:
            if (kc == KEY_LCTRL || kc == KEY_RCTRL) {
                bool ctrlDown = (CGEventGetFlags(event) & kCGEventFlagMaskControl) != 0;
                if (ctrlDown && !down) {
                    down = 1; usedAsMod = 0; activeKey = (int)kc; downAt = CFAbsoluteTimeGetCurrent();
                } else if (!ctrlDown && down && activeKey == (int)kc) {
                    double dt = CFAbsoluteTimeGetCurrent() - downAt;
                    if (dt <= tapThreshold && !usedAsMod) postEscape();
                    down = 0; usedAsMod = 0; activeKey = 0;
                }
            }
            break;
        case kCGEventKeyDown:
            if (down && !isModifier(kc)) usedAsMod = 1;
            break;
        default: break;
    }
    return event;
}

int main(void) {
    CGEventMask mask = (1 << kCGEventFlagsChanged) | (1 << kCGEventKeyDown);
    CFMachPortRef tap = CGEventTapCreate(kCGHIDEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, mask, callback, NULL);
    if (!tap) {
        fprintf(stderr, "[%s] Failed to create event tap\n", timestamp());
        return 1;
    }
    CFRunLoopSourceRef src = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
    if (!src) {
        fprintf(stderr, "[%s] Failed to create run loop source\n", timestamp());
        CFRelease(tap); return 1;
    }
    CFRunLoopAddSource(CFRunLoopGetCurrent(), src, kCFRunLoopCommonModes);
    CGEventTapEnable(tap, true);
    CFRunLoopRun();
    CFRelease(src);
    CFRelease(tap);
    return 0;
}
