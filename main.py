def calcula(x, y):
    return x + y
fim-def

a = 5
b = 10
c = 0
c = calcula(a, b)
print("%d %d %d" % (a, b, c))
a = calcula(7, a+b+c)
print("%s %d %d" % ("7", a+b+c, a))
c = calcula(a*b, a//b)
print("%d %d %d" % (a*b, a//b, c))
a = calcula(b, b+b)
msg = 'resultado ='
print("%s %d %d %d" % (msg, a, b, c))