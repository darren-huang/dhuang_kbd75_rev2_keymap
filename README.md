# To setup from new repo:
1. run
```
python3.8 -m pip install --user qmk
qmk setup
```

2. then edit the file `qmk_firmware/bin/qmk` and change on line 1:
```
-#!/usr/bin/env python3
+#!/usr/bin/env python3.8
```

3. then paste this repo inside `qmk_firmware/keyboards/kbdfans/kbd75/keymaps`

# To compile run
```
qmk compile -kb kbdfans/kbd75/rev2 -km darren-huang
```

# To flash to keyboard
use this software: (go to the releases page and get the exe): [qmk toolbox](https://github.com/qmk/qmk_toolbox)
