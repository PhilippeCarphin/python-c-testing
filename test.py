import iterator
import itertools

i = iterator.MyIterator()

print(next(i))
print("====================================")
print(next(i))
print("====================================")
print(next(i))

primes = [next(i) for k in range(8)]
print(primes)
primes = [t[0] for t in zip(i, range(8))]
print(list(primes))

