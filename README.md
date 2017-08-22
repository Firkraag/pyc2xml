# pyc2xml
解析pyc文件，输出xml

## 编译
gcc -m32 -O1 -o pyc2xml pyc2xml.c
因为python2.5.6版本long固定为四个字节，所以必须有-m32参数。

## 适用版本
python2.5.6
