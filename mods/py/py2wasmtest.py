import re
import sys
from nuitka.__main__ import py2wasm
if __name__ == '__main__':
    sys.argv[0] = re.sub(r'(-script\.pyw|\.exe)?$', '', sys.argv[0])
    sys.exit(py2wasm())