import random
serial = ''.join(random.choice('0123456789ABCDEF') for n in xrange(16))
id4 = ''.join(random.choice('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ') for n in xrange(4))
password = ''.join(random.choice('123456789') for n in xrange(3)) + '-' + ''.join(random.choice('123456789') for n in xrange(2)) + '-' + ''.join(random.choice('123456789') for n in xrange(3))

contents = """
#define PRODUCT_NAME "Dual Smart Outlet"
#define PRODUCT_4_ID "%s"
#define PRODUCT_FULL_NAME "%s"
#define SERIAL_NUMBER "%s"
#define MANUFACTURER "Aidan Lovelace"
#define PASSWORD "%s"
""" % (id4, "Dual Smart Outlet " + id4, serial, password)
# print(contents)
with open("product.h", "w") as f:
    f.write(contents)