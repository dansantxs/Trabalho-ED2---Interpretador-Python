def expressao(a, b):
    return a + b    
fim-def

def contagem_while():
    contador = 0
    print("Contagem usando while:")
    while contador < 5:
        print(contador)
        contador += 1
fim-def

def percorre_lista():
    lista = [10, 20, 30, 40]
    print("Percorrendo a lista usando for:")
    for item in lista:
        print(item)
fim-def

print("Bem-vindo ao programa de exemplo em Python!")

resultado = expressao(4, 7)
if resultado > 10:
    print("O resultado e maior que 10.")
elif resultado == 10:
    print("O resultado e exatamente 10.")
else:
    print("O resultado e menor que 10.")

lista = [5, 2, 9, 1]
print("Lista original:", lista)
    
lista.append(7)
print("Apos adicionar 7:", lista)
    
lista.remove(2)
print("Apos remover 2:", lista)
    
lista.sort()
print("Lista ordenada:", lista)
    
lista.pop()
print("Apos remover o ultimo elemento:", lista)

contagem_while()
percorre_lista()