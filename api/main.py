# -*- coding: utf8 -*-

import mapi
import sys

# In[]

if __name__ == "__main__":

  print("Default Encoding: {}".format(sys.getdefaultencoding()))
  
  print(mapi.get_long(12))
  print(mapi.get_int(1100))
  print(mapi.parse_args(1, 2, 'three'))
  print(mapi.int_add_mul(10, 20))
  print(mapi.get_map())
  print(mapi.get_advmap())
  print(mapi.get_list())

  file_path = "../docs/images/照片.jpg"
  file_exist = mapi.check_utf8_filepath(file_path.encode())
  print("Exists." if file_exist == 0 else "Not.")

  utf8_str = "This is a 字串."
  res = mapi.get_str(utf8_str.encode())
  print(type(res), res == utf8_str, utf8_str.encode())
