# Minimalist SSD1306 Display Library

Reduce footprint to under 9k for basic sample and make it compatible
with Adafruit_SSD1306 for graphics API as a fallback.

For ATmega328p only.

BitBlit functions for character and bitmap rendering are disabled via
`GFX_NO_BIT_BLIT` definition. This reduces the memory footprint by 856
bytes if both text and bitmap rendering functions are used, and 622
bytes if only text functions are used.

However, bit blit functions speed up rendering by a factor of 3.

|             `GFX_BIT_BLIT`  Enabled              |                  `GFX_NO_BIT_BLIT`                   |
|--------------------------------------------------|------------------------------------------------------|
| ![BitBlit_Bitmap.png](images/BitBlit_Bitmap.png) | ![NoBitBlit_Bitmap.png](images/NoBitBlit_Bitmap.png) |
| ![BitBlit_Text.png](images/BitBlit_Text.png)     | ![NoBitBlit_Text.png](images/NoBitBlit_Text.png)     |
| ![BitBlit_Both.jpeg](images/BitBlit_Both.png)    | ![NoBitBlit_Both.jpeg](images/NoBitBlit_Both.png)    |
| The numbers at the bottom of the images are uSec: Min/Avg/Max for rendering the bitmap and/or text.    ||

Sample GUI from my CookiePortioner project:

Options Menu (`PopupOptionsMenu`):  
![OptionsMenu](images/OptionsMenu.png)

Store/Recall Menu (`PopupOptionsMenu`): Shows current value stored in selected cell  
![StoreRecallMenu](images/StoreRecallMenu.png)

Configuration Menu (`ConfigForm`):  
![ConfigMenu](images/ConfigMenu.png)

Individual Values Setting Screen (`ConfigForm` & `FieldEditor`):  
![ValueSetting](images/ValueSetting.png)

After piston retraction, refill confirmation (`PopupMessage`):  
![RefillMessageBox](images/RefillMessageBox.png)
