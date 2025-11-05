#pragma once

namespace LindaEngine
{

#if _WIN32
    enum class KeyCode
    {
        None = -1,
        // ÍË¸ñ¼üºÍÌØÊâ¼ü
        BACK = 0x08,         // Backspace ¼ü
        TAB = 0x09,          // Tab ¼ü

        // Çå³ýºÍ»Ø³µ¼ü
        CLEAR = 0x0C,        // Clear ¼ü
        RETURN = 0x0D,       // Enter ¼ü

        // »»µµ¼ü
        SHIFT = 0x10,        // Shift ¼ü
        CONTROL = 0x11,      // Ctrl ¼ü
        MENU = 0x12,         // Alt ¼ü
        PAUSE = 0x13,        // Pause ¼ü
        CAPITAL = 0x14,      // Caps Lock ¼ü

        // IME ¼ü
        KANA = 0x15,         // IME Kana Ä£Ê½
        HANGUEL = 0x15,      // IME Hanguel Ä£Ê½
        HANGUL = 0x15,       // IME Hangul Ä£Ê½
        IME_ON = 0x16,       // IME ¿ªÆô
        JUNJA = 0x17,        // IME Junja Ä£Ê½
        FINAL = 0x18,        // IME final Ä£Ê½
        HANJA = 0x19,        // IME Hanja Ä£Ê½
        KANJI = 0x19,        // IME Kanji Ä£Ê½
        IME_OFF = 0x1A,      // IME ¹Ø±Õ

        // ×ªÒå¼ü
        ESCAPE = 0x1B,       // Esc ¼ü

        // IME ×ª»»
        CONVERT = 0x1C,      // IME convert
        NONCONVERT = 0x1D,   // IME nonconvert
        ACCEPT = 0x1E,       // IME accept
        MODECHANGE = 0x1F,   // IME Ä£Ê½±ä¸üÇëÇó

        // ¿Õ¸ñ¼ü
        SPACE = 0x20,        // ¿Õ¸ñ¼ü
        PRIOR = 0x21,        // Page Up ¼ü
        NEXT = 0x22,         // Page Down ¼ü
        END = 0x23,          // End ¼ü
        HOME = 0x24,         // Home ¼ü
        LEFT = 0x25,         // ×ó¼ýÍ·¼ü
        UP = 0x26,           // ÉÏ¼ýÍ·¼ü
        RIGHT = 0x27,        // ÓÒ¼ýÍ·¼ü
        DOWN = 0x28,         // ÏÂ¼ýÍ·¼ü
        SELECT = 0x29,       // Select ¼ü
        PRINT = 0x2A,        // Print ¼ü
        EXECUTE = 0x2B,      // Execute ¼ü
        SNAPSHOT = 0x2C,     // Print Screen ¼ü
        INSERT = 0x2D,       // Insert ¼ü
        Delete = 0x2E,       // Delete ¼ü
        HELP = 0x2F,         // Help ¼ü

        // Êý×Ö¼ü 0-9
        D0 = 0x30,            // 0 ¼ü
        D1 = 0x31,            // 1 ¼ü
        D2 = 0x32,            // 2 ¼ü
        D3 = 0x33,            // 3 ¼ü
        D4 = 0x34,            // 4 ¼ü
        D5 = 0x35,            // 5 ¼ü
        D6 = 0x36,            // 6 ¼ü
        D7 = 0x37,            // 7 ¼ü
        D8 = 0x38,            // 8 ¼ü
        D9 = 0x39,            // 9 ¼ü

        // ×ÖÄ¸¼ü A-Z
        A = 0x41,            // A ¼ü
        B = 0x42,            // B ¼ü
        C = 0x43,            // C ¼ü
        D = 0x44,            // D ¼ü
        E = 0x45,            // E ¼ü
        F = 0x46,            // F ¼ü
        G = 0x47,            // G ¼ü
        H = 0x48,            // H ¼ü
        I = 0x49,            // I ¼ü
        J = 0x4A,            // J ¼ü
        K = 0x4B,            // K ¼ü
        L = 0x4C,            // L ¼ü
        M = 0x4D,            // M ¼ü
        N = 0x4E,            // N ¼ü
        O = 0x4F,            // O ¼ü
        P = 0x50,            // P ¼ü
        Q = 0x51,            // Q ¼ü
        R = 0x52,            // R ¼ü
        S = 0x53,            // S ¼ü
        T = 0x54,            // T ¼ü
        U = 0x55,            // U ¼ü
        V = 0x56,            // V ¼ü
        W = 0x57,            // W ¼ü
        X = 0x58,            // X ¼ü
        Y = 0x59,            // Y ¼ü
        Z = 0x5A,            // Z ¼ü

        // Windows ¼ü
        LWIN = 0x5B,         // ×ó Windows ¼ü
        RWIN = 0x5C,         // ÓÒ Windows ¼ü
        APPS = 0x5D,         // Ó¦ÓÃ³ÌÐò¼ü

        // ÏµÍ³Ë¯Ãß¼ü
        SLEEP = 0x5F,        // ¼ÆËã»úË¯Ãß¼ü

        // Ð¡¼üÅÌ¼ü
        KP0 = 0x60,      // Ð¡¼üÅÌ 0 ¼ü
        KP1 = 0x61,      // Ð¡¼üÅÌ 1 ¼ü
        KP2 = 0x62,      // Ð¡¼üÅÌ 2 ¼ü
        KP3 = 0x63,      // Ð¡¼üÅÌ 3 ¼ü
        KP4 = 0x64,      // Ð¡¼üÅÌ 4 ¼ü
        KP5 = 0x65,      // Ð¡¼üÅÌ 5 ¼ü
        KP6 = 0x66,      // Ð¡¼üÅÌ 6 ¼ü
        KP7 = 0x67,      // Ð¡¼üÅÌ 7 ¼ü
        KP8 = 0x68,      // Ð¡¼üÅÌ 8 ¼ü
        KP9 = 0x69,      // Ð¡¼üÅÌ 9 ¼ü
        MULTIPLY = 0x6A,     // ³ËºÅ¼ü
        ADD = 0x6B,          // ¼ÓºÅ¼ü
        SEPARATOR = 0x6C,    // ·Ö¸ô·û¼ü
        SUBTRACT = 0x6D,     // ¼õºÅ¼ü
        DECIMAL = 0x6E,      // Ð¡Êýµã¼ü
        DIVIDE = 0x6F,       // ³ýºÅ¼ü

        // ¹¦ÄÜ¼ü
        F1 = 0x70,           // F1 ¼ü
        F2 = 0x71,           // F2 ¼ü
        F3 = 0x72,           // F3 ¼ü
        F4 = 0x73,           // F4 ¼ü
        F5 = 0x74,           // F5 ¼ü
        F6 = 0x75,           // F6 ¼ü
        F7 = 0x76,           // F7 ¼ü
        F8 = 0x77,           // F8 ¼ü
        F9 = 0x78,           // F9 ¼ü
        F10 = 0x79,          // F10 ¼ü
        F11 = 0x7A,          // F11 ¼ü
        F12 = 0x7B,          // F12 ¼ü
        F13 = 0x7C,          // F13 ¼ü
        F14 = 0x7D,          // F14 ¼ü
        F15 = 0x7E,          // F15 ¼ü
        F16 = 0x7F,          // F16 ¼ü
        F17 = 0x80,          // F17 ¼ü
        F18 = 0x81,          // F18 ¼ü
        F19 = 0x82,          // F19 ¼ü
        F20 = 0x83,          // F20 ¼ü
        F21 = 0x84,          // F21 ¼ü
        F22 = 0x85,          // F22 ¼ü
        F23 = 0x86,          // F23 ¼ü
        F24 = 0x87,          // F24 ¼ü

        // µ¼º½Ëø¼ü
        NUMLOCK = 0x90,      // Num Lock ¼ü
        SCROLL = 0x91,       // Scroll Lock ¼ü

        // »»µµ¼ü³£Á¿
        LSHIFT = 0xA0,       // ×ó Shift ¼ü
        RSHIFT = 0xA1,       // ÓÒ Shift ¼ü
        LCONTROL = 0xA2,     // ×ó Ctrl ¼ü
        RCONTROL = 0xA3,     // ÓÒ Ctrl ¼ü
        LMENU = 0xA4,        // ×ó Alt ¼ü
        RMENU = 0xA5,        // ÓÒ Alt ¼ü

        // ä¯ÀÀÆ÷¼ü
        BROWSER_BACK = 0xA6,         // ä¯ÀÀÆ÷ºóÍË¼ü
        BROWSER_FORWARD = 0xA7,      // ä¯ÀÀÆ÷Ç°½ø¼ü
        BROWSER_REFRESH = 0xA8,      // ä¯ÀÀÆ÷Ë¢ÐÂ¼ü
        BROWSER_STOP = 0xA9,         // ä¯ÀÀÆ÷Í£Ö¹¼ü
        BROWSER_SEARCH = 0xAA,       // ä¯ÀÀÆ÷ËÑË÷¼ü
        BROWSER_FAVORITES = 0xAB,    // ä¯ÀÀÆ÷ÊÕ²Ø¼ü
        BROWSER_HOME = 0xAC,         // ä¯ÀÀÆ÷Ö÷Ò³¼ü

        // ÒôÁ¿¼ü
        VOLUME_MUTE = 0xAD,          // ÒôÁ¿¾²Òô¼ü
        VOLUME_DOWN = 0xAE,          // ÒôÁ¿¼õÐ¡¼ü
        VOLUME_UP = 0xAF,            // ÒôÁ¿Ôö´ó¼ü

        // Ã½Ìå¼ü
        MEDIA_NEXT_TRACK = 0xB0,     // ÏÂÒ»ÇúÄ¿¼ü
        MEDIA_PREV_TRACK = 0xB1,     // ÉÏÒ»ÇúÄ¿¼ü
        MEDIA_STOP = 0xB2,           // Í£Ö¹Ã½Ìå¼ü
        MEDIA_PLAY_PAUSE = 0xB3,     // ²¥·Å/ÔÝÍ£Ã½Ìå¼ü

        // Ó¦ÓÃ³ÌÐòÆô¶¯¼ü
        LAUNCH_MAIL = 0xB4,          // Æô¶¯ÓÊ¼þ¼ü
        LAUNCH_MEDIA_SELECT = 0xB5,  // Ñ¡ÔñÃ½Ìå¼ü
        LAUNCH_APP1 = 0xB6,          // Æô¶¯Ó¦ÓÃ³ÌÐò1¼ü
        LAUNCH_APP2 = 0xB7,          // Æô¶¯Ó¦ÓÃ³ÌÐò2¼ü

        // OEM ÌØ¶¨¼ü
        OEM_1 = 0xBA,        // ;: ¼ü£¨US±ê×¼¼üÅÌ£©
        OEM_PLUS = 0xBB,     // + ¼ü
        OEM_COMMA = 0xBC,    // , ¼ü
        OEM_MINUS = 0xBD,    // - ¼ü
        OEM_PERIOD = 0xBE,   // . ¼ü
        OEM_2 = 0xBF,        // /? ¼ü£¨US±ê×¼¼üÅÌ£©
        OEM_3 = 0xC0,        // `~ ¼ü£¨US±ê×¼¼üÅÌ£©

        OEM_4 = 0xDB,        // [{ ¼ü£¨US±ê×¼¼üÅÌ£©
        OEM_5 = 0xDC,        // \| ¼ü£¨US±ê×¼¼üÅÌ£©
        OEM_6 = 0xDD,        // ]} ¼ü£¨US±ê×¼¼üÅÌ£©
        OEM_7 = 0xDE,        // '" ¼ü£¨US±ê×¼¼üÅÌ£©
        OEM_8 = 0xDF,        // ÓÃÓÚÔÓÏî×Ö·û

        // OEM ÌØ¶¨¼ü
        OEM_102 = 0xE2,      // <> ¼ü»ò \| ¼ü£¨RT 102¼ü¼üÅÌ£©

        // IME Ïà¹Ø¼ü
        PROCESSKEY = 0xE5,   // IME ´¦Àí¼ü

        // OEM ÌØ¶¨¼ü
        PACKET = 0xE7,       // ÓÃÓÚ´«µÝUnicode×Ö·û

        // ¸½¼Ó¼ü
        ATTN = 0xF6,         // Attn ¼ü
        CRSEL = 0xF7,        // CrSel ¼ü
        EXSEL = 0xF8,        // ExSel ¼ü
        EREOF = 0xF9,        // Erase EOF ¼ü
        PLAY = 0xFA,         // Play ¼ü
        ZOOM = 0xFB,         // Zoom ¼ü
        NONAME = 0xFC,       // ±£Áô
        PA1 = 0xFD,          // PA1 ¼ü
        OEM_CLEAR = 0xFE     // Clear ¼ü
    };
#elif
    enum class KeyCode
    {
        None = -1,
        // From glfw3.h
        Space = 32,
        Apostrophe = 39, /* ' */
        Comma = 44, /* , */
        Minus = 45, /* - */
        Period = 46, /* . */
        Slash = 47, /* / */

        D0 = 48, /* 0 */
        D1 = 49, /* 1 */
        D2 = 50, /* 2 */
        D3 = 51, /* 3 */
        D4 = 52, /* 4 */
        D5 = 53, /* 5 */
        D6 = 54, /* 6 */
        D7 = 55, /* 7 */
        D8 = 56, /* 8 */
        D9 = 57, /* 9 */

        Semicolon = 59, /* ; */
        Equal = 61, /* = */

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,  /* [ */
        Backslash = 92,  /* \ */
        RightBracket = 93,  /* ] */
        GraveAccent = 96,  /* ` */

        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        /* Function keys */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        /* Keypad */
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };
#endif
	enum class MouseCode
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};

}