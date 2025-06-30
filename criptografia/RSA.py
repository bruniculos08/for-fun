# -*- coding: utf-8 -*-

def GCD(a, b):
    if(b == 0):
        return a
    else:
        a, b = b, (a % b)
        return GCD(a,b)

def Coprime(phi):
    for i in range(2, phi):
        if(GCD(i, phi) == 1):
            return i
    return 1

def InverseModulo(a, phi):
    for i in range(3, phi):
        if((a * i) % phi == 1):
            return i
    return a

p = 41
q = 47
N = p * q
phi = (p - 1) * (q - 1)

s = Coprime(phi)
r = InverseModulo(s, phi) 
# r * s mod phi = 1

mensagem = 73
mensagem_publicada = (mensagem ** s) % N
mensagem_recebida = (mensagem_publicada ** r) % N 

print(mensagem_publicada)
print(mensagem_recebida)

# Why it works:
# 
# (1) p and q are prime numbers
# 
# (2) N = p * q
# 
# (3) φ(N) is Touter-Euler Function applied in N, i.e., φ(N) is the number of coprimes of N that are in the...
# ... range [1, n]; note that, if m is prime than φ(m^k) = m^k - m^(k-1) because there are m^(k-1) multiples...
# ... of m in the range [1, m-1] what can be seen in the following line of multiples of p inside the range:
#       
#    p*1  p*2  p*3  p*4  ...  p*(p-1)  p*p  p*(p+1)  p*(p+2)  ...  p*(p²-1)  p*(p²)  p*(p²+1)  p*(p²+2)  ...  p*(p^(k-1)-1)  p*(p^(k-1))
# 
# It can be proved also that for m and n primes, φ(m * n) = φ(m) * φ(n), then, φ(m * n) = (m-1) * (n-1).
# Obs.: for any prime m, φ(m) = m-1
# 
# (4) Euler-Fermat's Theorem: let a and m two integers with m > 0 and gcd(a, m) = 1, then
#  
#   a^(φ(m)) ≡ 1  (mod m)
# 
# (4) Fermat's Little Theorem: let a and m with m being a prime, then
# 
#   a^p ≡ a (mod p)
# 
# (4) gcd(s, φ(N)) = gcd(s, (p-1) * (q-1)) = 1, therefore exists r such that:
#  
#       (4.1) r < (p-1) * (q-1)
# 
#       (4.2) (r*s) ≡ 1  (mod φ(N)), then r*s - 1 = k*φ(N) for some integer k
# 
# (5) Supose the emiter wants to send a number m, then let
# 
#       x = (m^s mod N)
#
# Let's proof that:
# 
#       x^r ≡ m  (mod N)
# 
# Case gcd(m, N) = 1: using Euler-Fermat's Theorem we have
# 
#       m^φ(N) ≡ 1 (mod N)  =>  (m^φ(N))^k ≡ 1^k (mod N)
#                           =>  m^(k*φ(N)) ≡ 1 (mod N) 
# 
# Because gcd(m, N) = 1:
# 
#       m^φ(N) ≡ 1 (mod N)  => m * m^(k*φ(N)) ≡ m (mod N)
#                           => m * m^(r*s - 1) ≡ m (mod N)
#                           => m * m^(r*s) * m^(-1) ≡ m (mod N)
#                           => m^(r*s) ≡ m (mod N)
# 
# Case gcd(m, N) ≠ 1: using a result from Chinese Remainder Theorem, by the fact ...
# ... that p and q are primes therefore gcd(p, q) = 1, we have that
# 
#     x ≡ y  (mod p)  ∧  x ≡ y  (mod q)  ⇒  x ≡ y  (mod p*q)
# 
# So, we must prove that:
# 
#   (m^s)^r ≡ m  (mod p)
# 
#   (m^s)^r ≡ m  (mod q)
# 
# Proving the first statement, we have the following cases, considering the fact that p is prime:
# 
#       (1) p divides m, then both sides of the equation are 0
# 
#       (2) m = 1, then both sides of the equation are 1
# 
#       (2) gcd(m, p) = 1, then we can use Euler-Fermat's Theorem, which gives us:
# 
#                  m^(φ(p)) ≡ 1  (mod p)
#                  m^(p-1) ≡ 1  (mod p)
# 
#       Using modular arithmetic we can raise both sides to the power of k*(q-1) where r*s - 1 = k*φ(N), ...
#       ... i.e., r*s = k*(p-1)*(q-1) + 1, then we get: 
# 
#                  m^(k*φ(N)) ≡ 1  (mod p)
# 
#       Using again modular arithmetic, because gcd(m, p) = 1, we can get:
# 
#                  m*m^(k*φ(N)) ≡ m  (mod p)
#                  m^(k*φ(N) + 1) ≡ m  (mod p)
#                  m^(r*s) ≡ m  (mod p)
# 
# For proving the second statement the process is identical.

