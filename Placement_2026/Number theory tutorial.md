# Number Theory for Competitive Programming

A complete write-up of the Maths_5 and Maths_6 lecture notes.

**Syllabus**

1. [Primes and counting factors](#1-primes-and-counting-factors)
2. [Sieve of Eratosthenes](#2-sieve-of-eratosthenes)
3. [Segmented Sieve](#3-segmented-sieve)
4. [Prime factorization and SPF](#4-prime-factorization-and-spf)
5. [Number of divisors of N](#5-number-of-divisors-of-n)
6. [Euclid's algorithm for GCD](#6-euclids-algorithm-for-gcd)
7. [Extended Euclidean algorithm](#7-extended-euclidean-algorithm)
8. [Modular arithmetic and modular inverse](#8-modular-arithmetic-and-modular-inverse)
9. [Binomial coefficients mod p](#9-binomial-coefficients-mod-p)
10. [Inclusion–Exclusion](#10-inclusionexclusion)
11. [Euler's Totient function](#11-eulers-totient-function)
12. [Worked problem: sum of GCD over all pairs](#12-worked-problem-sum-of-gcd-over-all-pairs)

---

## 1. Primes and counting factors

A prime has exactly **two** factors: 1 and itself. So "is `n` prime?" is really "does `n` have exactly 2 factors?"

### Attempt 1 — check everything: O(n)

```cpp
int countFactors(int n) {
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        if (n % i == 0) cnt++;
    return cnt;
}
```

### Attempt 2 — stop at n/2: O(n/2)

No factor of `n` other than `n` itself can exceed `n/2`. For `n = 100`, nothing in 51..99 divides it. But O(n/2) is still O(n) — a constant factor, not a real win.

### Attempt 3 — factors come in pairs: O(√n)

This is the key idea. Write out the divisors of 100:

```
1    2    4    5    10    20    25    50    100
└────┴────┴────┴─────┼─────┴─────┴────┴─────┘
                pairs meet at 10 = √100
```

**If `i` divides `n`, then `n/i` also divides `n`.** So divisors come in pairs `(i, n/i)`, and one member of each pair is always ≤ √n. Loop only to √n and count both members:

```cpp
bool isPrime(long long n) {
    if (n < 2) return false;
    for (long long i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}
```

> Use `i * i <= n` rather than `i <= sqrt(n)`. `sqrt` returns a `double` and can round wrong at large values.

The pair `(10, 10)` for `n = 100` is a single divisor, not two — that is why perfect squares have an **odd** number of divisors.

### The problem this doesn't solve

> Find the number of primes between `L` and `R`, where `L = 0`, `R = 10⁶`.

- Outer loop over the range: O(R)
- Primality check for each: O(√k)

Total ≈ 10⁶ × 10³ = **10⁹ operations → TLE**.

The fix isn't a faster check. It's to stop asking one number at a time.

---

## 2. Sieve of Eratosthenes

Precompute once, then answer each query in **O(1)**:

```
        ┌─────────────────┐      prime
   n ───│   lookup table  │───▶  or
        └─────────────────┘      not prime
```

Build a boolean array where `isPrime[i]` tells you the answer directly:

```
index:  0  1  2  3  4  5  6  7  8  9 10 11 12 ...
value:  F  F  T  T  F  T  F  T  F  F  F  T  F
```

### The algorithm

Walk `i` upward. When `i` is still marked prime, cross out every multiple of `i`.

```cpp
const int N = 1e6;
vector<bool> isPrime(N + 1, true);

void sieve() {
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; (long long)i * i <= N; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= N; j += i)
                isPrime[j] = false;
        }
    }
}
```

Two details that are easy to get wrong:

- **Start the inner loop at `i * i`, not `2 * i`.** Every smaller multiple of `i` — `2i`, `3i`, … up to `(i-1)i` — already had a smaller prime factor and was crossed out earlier.
- **Size the array `N + 1`, not `N`,** so index `N` is valid.

### Complexity

Building the table is **O(n log log n)** — near linear. Answering the range query afterwards is O(R − L). The 10⁹ figure collapses to about 10⁶.

---

## 3. Segmented Sieve

Now change the constraints:

```
1 ≤ L ≤ R ≤ 10⁹        and        R − L ≤ 10⁶
```

A plain sieve up to 10⁹ needs an array of a billion entries. Too much memory. But the **window is small** — at most 10⁶ numbers.

The trick rests on one fact: any composite `n ≤ R` has a prime factor ≤ √R. With R = 10⁹, that's only 31623. So:

1. Sieve normally up to √R to collect the "small" primes.
2. Make a boolean array of size `R − L + 1`, indexed by `offset = value − L`.
3. For each small prime `p`, cross out its multiples **inside the window**.

```cpp
vector<bool> segmentedSieve(long long L, long long R) {
    long long lim = sqrt((long double)R) + 1;

    // step 1: small primes up to sqrt(R)
    vector<bool> mark(lim + 1, true);
    vector<long long> primes;
    for (long long i = 2; i <= lim; i++) {
        if (mark[i]) {
            primes.push_back(i);
            for (long long j = i * i; j <= lim; j += i) mark[j] = false;
        }
    }

    // step 2: the window, shifted so that index 0 means the value L
    vector<bool> isPrime(R - L + 1, true);
    for (long long p : primes) {
        // first multiple of p that is >= L, but never below p*p
        long long start = max(p * p, (L + p - 1) / p * p);
        for (long long j = start; j <= R; j += p)
            isPrime[j - L] = false;
    }

    if (L <= 1) for (long long v = L; v <= min(R, 1LL); v++) isPrime[v - L] = false;
    return isPrime;
}
```

The line `(L + p - 1) / p * p` is ceiling division — it rounds `L` up to the next multiple of `p`. The `j - L` offset is what keeps memory proportional to the window instead of to `R`.

---

## 4. Prime factorization and SPF

Every integer > 1 factors uniquely into primes:

```
100 = 2² × 5²          11 = 11¹          x = p₁^a₁ × p₂^a₂ × p₃^a₃ × …
```

### Single number: O(√n)

Divide out each prime as far as it goes:

```cpp
vector<pair<long long,int>> factorize(long long n) {
    vector<pair<long long,int>> f;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int c = 0;
            while (n % p == 0) { n /= p; c++; }
            f.push_back({p, c});
        }
    }
    if (n > 1) f.push_back({n, 1});   // leftover prime > sqrt(original n)
    return f;
}
```

That final `if` matters. After the loop, whatever is left is either 1 or a single prime larger than √n — for example factoring 14 leaves 7 behind.

Tracing 100 by hand:

```
100 / 2 = 50
 50 / 2 = 25       →   2 appears twice
 25 / 5 = 5
  5 / 5 = 1        →   5 appears twice
```

### Many numbers: the SPF sieve

If you must factor thousands of numbers, O(√n) each is too slow. Instead precompute the **smallest prime factor** of every number, using the same sieve shape:

```cpp
const int N = 1e6;
int spf[N + 1];

void buildSPF() {
    for (int i = 0; i <= N; i++) spf[i] = i;
    for (int i = 2; (long long)i * i <= N; i++)
        if (spf[i] == i)                       // i is prime
            for (int j = i * i; j <= N; j += i)
                if (spf[j] == j) spf[j] = i;   // only if not already set
}
```

`if (spf[j] == j)` guarantees we keep the *smallest* factor — later, larger primes won't overwrite it.

Factoring then becomes a chain of lookups:

```
spf[100] = 2  →  100/2 = 50
spf[50]  = 2  →   50/2 = 25
spf[25]  = 5  →   25/5 = 5
spf[5]   = 5  →    5/5 = 1
```

```cpp
vector<pair<int,int>> fastFactorize(int x) {
    vector<pair<int,int>> f;
    while (x > 1) {
        int p = spf[x], c = 0;
        while (x % p == 0) { x /= p; c++; }
        f.push_back({p, c});
    }
    return f;
}
```

**O(log n) per number** after the precompute.

### Application: reducing a fraction without dividing

Factor numerator and denominator, then subtract exponents. `100/6`:

```
100     2² × 5²        2 × 5²
─── = ─────────── = ───────────
  6     2¹ × 3¹          3
```

This works even when the numbers are products too large to store.

---

## 5. Number of divisors of N

From the factorization, the divisor count follows immediately. If

```
x = p₁^a₁ × p₂^a₂ × p₃^a₃ × …
```

then any divisor picks an exponent for `p₁` from `{0, 1, …, a₁}` — that's `a₁ + 1` choices — and independently for each other prime. So:

```
d(x) = (a₁ + 1) × (a₂ + 1) × (a₃ + 1) × …
```

For 100 = 2² × 5²:  `(2+1) × (2+1) = 9`. Check: 1, 2, 4, 5, 10, 20, 25, 50, 100. Nine.

```cpp
long long countDivisors(long long n) {
    long long d = 1;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int c = 0;
            while (n % p == 0) { n /= p; c++; }
            d *= (c + 1);
        }
    }
    if (n > 1) d *= 2;
    return d;
}
```

### T-primes

A **T-prime** has exactly 3 divisors. Using the formula, we need

```
(a₁ + 1)(a₂ + 1)(a₃ + 1) … = 3
```

3 is prime, so exactly one bracket is 3 and all the rest are 1. That means one exponent is 2 and every other is 0:

```
x = p²
```

**A number is a T-prime exactly when it is the square of a prime.** 4 = 2² → {1,2,4}. 9 = 3² → {1,3,9}. 25 = 5² → {1,5,25}. To test `x`: check `√x` is an integer and that integer is prime.

---

## 6. Euclid's algorithm for GCD

The naive approach lists all factors of A, all factors of B, and takes the largest shared one. Far too slow.

### The subtraction identity

```
GCD(A, B) = GCD(A, B − A) = GCD(A − B, B)
```

Any common divisor of A and B also divides their difference, so the set of common divisors — and therefore the greatest one — never changes.

```
GCD(12, 24) = GCD(12, 24 − 12) = GCD(12, 12) = GCD(12, 0) = 12
```

When one side reaches 0, the other is the answer: `GCD(a, 0) = a`.

### Speeding it up with mod

Repeated subtraction is slow. `GCD(100, 2)` would take 50 steps: 98, 96, 94… Subtracting `B` from `A` over and over *is* the mod operation, so do it in one step:

```
GCD(A, B) = GCD(B, A mod B)
```

```cpp
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}
```

**O(log(min(a, b)))**. In C++17, `std::__gcd(a, b)` or `std::gcd` from `<numeric>` is built in.

---

## 7. Extended Euclidean algorithm

Plain GCD returns one number. The extended version also returns the **coefficients**, which unlocks two things:

- Multiplicative inverse
- Linear Diophantine equations

**Goal:** given A and B, find `x` and `y` with

```
Ax + By = GCD(A, B)
```

### Deriving the recursion

Assume the recursive call on `(B, A mod B)` already gave us `x₁, y₁`:

```
B·x₁ + (A mod B)·y₁ = GCD(B, A mod B)
```

Substitute the definition of mod, `A mod B = A − ⌊A/B⌋·B`:

```
B·x₁ + [A − ⌊A/B⌋·B]·y₁ = GCD(B, A mod B)
B·x₁ + A·y₁ − ⌊A/B⌋·B·y₁ = GCD(B, A mod B)
```

Group the B terms:

```
B·[x₁ − ⌊A/B⌋·y₁] + A·y₁ = GCD(B, A mod B)
```

And since `GCD(A, B) = GCD(B, A mod B)`, the right side is just `GCD(A, B)`. Compare with the target `Ax + By = GCD(A,B)`:

```
┌────────────────────────────────────┐
│   x = y₁          y = x₁ − ⌊A/B⌋·y₁ │
└────────────────────────────────────┘
```

Base case: when `B = 0`, `A·1 + 0·0 = A`, so `x = 1, y = 0, g = A`.

```cpp
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
```

### Worked example: A = 38, B = 12

Recurse down to the base case, then rebuild upward:

| call | a | b | returns (x, y) |
|---|---|---|---|
| 3 | 2 | 0 | (1, 0) |
| 2 | 12 | 2 | x = 0, y = 1 − ⌊12/2⌋·0 = 1 → (0, 1) |
| 1 | 38 | 12 | x = 1, y = 0 − ⌊38/12⌋·1 = −3 → (1, −3) |

Check: `38(1) + 12(−3) = 38 − 36 = 2 = GCD(38, 12)`. ✓

### Second example: A = 18, B = 30

| a | b | (x, y) |
|---|---|---|
| 6 | 0 | (1, 0) |
| 12 | 6 | (0, 1) |
| 18 | 12 | (1, −1) |
| 30 | 18 | (−1, 2) |
| 18 | 30 | (2, −1) |

Check: `18(2) + 30(−1) = 36 − 30 = 6 = GCD(18, 30)`. ✓

---

## 8. Modular arithmetic and modular inverse

### Why mod at all

Counting answers explode. A problem with `n = 10⁶` can easily have an answer near 10⁹ or far beyond — well past what a 64-bit integer holds. So problems ask for the answer **mod 10⁹+7**, which keeps every value under 10⁹ and makes overflow manageable.

Why 10⁹+7: it is prime (which we need for inverses), and its square still fits in a signed 64-bit `long long`.

### The rules

```
(a + b) % m = ((a % m) + (b % m)) % m
(a − b) % m = ((a % m) − (b % m) + m) % m
(a × b) % m = ((a % m) × (b % m)) % m
```

The `+ m` in subtraction is essential. In C++, `-1 % 5` is `-1`, not `4`. Example with `a = 8, b = 4, m = 5`:

```
(8%5 − 4%5 + 5) % 5 = (3 − 4 + 5) % 5 = 4 % 5 = 4
```

Without `+ m` you'd get `-1`, and using that as an array index is a crash.

### Division breaks

```
(a / b) % m  ≠  ((a % m) / (b % m)) % m        ✗
```

Division has no modular counterpart. Instead, multiply by the **multiplicative inverse**.

### Multiplicative inverse

Over the reals, `5 × 1/5 = 1`, i.e. `A × A⁻¹ = 1`. Under a modulus, `A⁻¹` is the integer `x` with

```
A · x ≡ 1 (mod m)
```

For `A = 3, m = 10`: try x = 1, 2, 3, …, and x = 7 works, since `21 % 10 = 1`. So `3⁻¹ ≡ 7 (mod 10)`.

**It doesn't always exist.** Take `A = 10, m = 5`: `(10x) % 5` is always 0, never 1. The condition is

```
GCD(A, m) = 1        (A and m must be coprime)
```

Here `GCD(10, 5) = 5 ≠ 1`, so no inverse. Once you have the inverse, division becomes multiplication:

```
(A / B) % m  =  (A × B⁻¹) % m  =  ((A % m) × (B⁻¹ % m)) % m
```

### Method 1 — from extended Euclid

Start from what the inverse means, `A·B ≡ 1 (mod m)`, and write out the division:

```
A·B = Q·m + R      where the remainder R is 1
A·B = Q·m + 1
A·B − Q·m = 1
A·B + (−Q)·m = 1
```

Let `Q' = −Q`:

```
A·B + m·Q' = 1
```

Now compare with the extended Euclid form:

```
A·x + m·y = GCD(A, m) = 1
```

They are the same equation. **The `x` that extended Euclid returns is the inverse of A.** That takes the search from O(m) down to O(log m).

```cpp
long long modInverse(long long a, long long m) {
    long long x, y;
    long long g = extgcd(a, m, x, y);
    if (g != 1) return -1;          // no inverse exists
    return (x % m + m) % m;         // normalise into [0, m)
}
```

The final `(x % m + m) % m` matters — extended Euclid often returns a negative `x`.

### Method 2 — Fermat's little theorem

When `m` is **prime** (as 10⁹+7 is) and `a` is not a multiple of it:

```
a^(m−1) ≡ 1 (mod m)      ⟹      a⁻¹ ≡ a^(m−2) (mod m)
```

This needs fast exponentiation.

### Binary exponentiation

Computing `a^b` by multiplying `b` times is O(b) — hopeless for b ≈ 10⁹. Instead use the binary form of the exponent. For `2⁵`, since `5 = 101₂`:

```
        2³  2²  2¹  2⁰       ← bit positions
 5  =    1   0   1
         ↓       ↓
2⁵  =   2⁴  ×   2¹  =  16 × 2  =  32
```

And `2¹⁰`, where `10 = 1010₂`:

```
2¹⁰ = 2⁸ × 2² = 256 × 4 = 1024
```

Each squaring gets you to the next power of two: `2 → 4 → 16 → 256`. Only ~log₂(b) multiplications.

```cpp
long long power(long long a, long long b, long long m) {
    long long res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = res * a % m;   // this bit is set: take it
        a = a * a % m;                  // move to the next power
        b >>= 1;
    }
    return res;
}

long long modInversePrime(long long a, long long m) {
    return power(a, m - 2, m);
}
```

**O(log b)**. Keep everything `long long`: with m ≈ 10⁹, the product `res * a` reaches ~10¹⁸, which fits in a signed 64-bit integer but overflows a 32-bit `int`.

---

## 9. Binomial coefficients mod p

```
C(n, r) = n! / (r! · (n−r)!)
```

There's a division, so we need inverses. Precompute factorials and their inverses once:

```cpp
const int MOD = 1e9 + 7;
const int MX  = 1e6;
long long fact[MX + 1], invFact[MX + 1];

void buildFactorials() {
    fact[0] = 1;
    for (int i = 1; i <= MX; i++) fact[i] = fact[i-1] * i % MOD;

    invFact[MX] = power(fact[MX], MOD - 2, MOD);
    for (int i = MX; i >= 1; i--) invFact[i-1] = invFact[i] * i % MOD;
}

long long C(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n-r] % MOD;
}
```

The backward loop is the trick worth remembering. Since `invFact[i] = 1/i!` and `1/(i−1)! = (1/i!) × i`, one modular exponentiation at the top gives every inverse factorial for free — **O(n) total** instead of O(n log MOD).

---

## 10. Inclusion–Exclusion

> Given primes `P = {p₁, …, p_k}` and a bound `n`, how many integers in [1, n] are divisible by at least one of them?

The brute force checks every number against every prime:

```cpp
for (int i = 2; i <= n; i++) {
    for (int j = 0; j < k; j++)
        if (i % P[j] == 0) { cnt++; break; }
}
```

O(n × k). With `n = 10¹⁸` that's hopeless — and note `n` here is a *bound*, not an array size, so we can't iterate it at all.

### The principle

Count multiples of each prime with `n / p`, but overlaps get counted twice:

```
         ┌───────┐
     ┌───┼───┐   │
     │ A │ ∩ │ B │
     └───┼───┘   │
         └───────┘

|A ∪ B| = |A| + |B| − |A ∩ B|
```

For `n = 20`, A = multiples of 2, B = multiples of 5:

```
⌊20/2⌋ + ⌊20/5⌋ − ⌊20/(2×5)⌋ = 10 + 4 − 2 = 12
```

The intersection is the multiples of `2 × 5`, because for distinct primes the LCM is just the product.

With three sets:

```
|A ∪ B ∪ C| = |A| + |B| + |C|
            − |A∩B| − |A∩C| − |B∩C|
            + |A∩B∩C|
```

**Odd-sized groups add, even-sized groups subtract.**

For `n = 50` with primes 2, 3, 5:

```
  50/2  + 50/3  + 50/5      =  25 + 16 + 10  =  51
− 50/6  − 50/10 − 50/15     = −  8 −  5 −  3 = −16
+ 50/30                     = + 1
                            ─────────────────────
                                             36
```

### Implementation: enumerate subsets with a bitmask

With `k` primes there are `2^k` subsets. Bit `i` of `mask` means "prime `i` is in this subset":

```
mask  bits   subset      term
  0   000    { }         skip
  1   001    {A}         + n/A
  2   010    {B}         + n/B
  3   011    {A,B}       − n/(A×B)
  4   100    {C}         + n/C
  5   101    {A,C}       − n/(A×C)
  6   110    {B,C}       − n/(B×C)
  7   111    {A,B,C}     + n/(A×B×C)
```

The sign is decided by the popcount: odd → `+`, even → `−`.

```cpp
long long countDivisibleByAny(long long n, vector<long long>& p) {
    int k = p.size();
    long long total = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        long long prod = 1;
        int bits = 0;
        for (int i = 0; i < k; i++) {
            if (mask >> i & 1) { prod *= p[i]; bits++; }
        }
        total += (bits % 2 ? 1 : -1) * (n / prod);
    }
    return total;
}
```

**O(2^k × k)** — independent of `n`, so 10¹⁸ is fine as long as `k` stays around 20 or less. Watch for overflow in `prod` when many large primes multiply together; break out early once `prod > n`, since the term contributes 0.

---

## 11. Euler's Totient function

**φ(n)** counts the integers in [1, n] that are coprime to `n` — that is, how many `x` satisfy `GCD(n, x) = 1`.

For `n = 10`, strike out anything sharing a factor with 10:

```
1  2̶  3  4̶  5̶  6̶  7  8̶  9  1̶0̶     →   {1, 3, 7, 9}   →   φ(10) = 4
```

### The formula

```
φ(n) = n × (1 − 1/p₁) × (1 − 1/p₂) × (1 − 1/p₃) × …
```

over the **distinct** prime factors. Since `10 = 2 × 5`:

```
φ(10) = 10 × (1 − 1/2) × (1 − 1/5) = 10 × 1/2 × 4/5 = 4  ✓
```

The intuition is inclusion–exclusion again: throw away the fraction `1/p` of numbers divisible by each prime.

```cpp
long long phi(long long n) {
    long long res = n;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            res -= res / p;              // res *= (1 - 1/p)
        }
    }
    if (n > 1) res -= res / n;
    return res;
}
```

`res -= res / p` is integer-safe multiplication by `(1 − 1/p)` — no floating point.

### Totient sieve — all values up to N

Same shape as the ordinary sieve. Seed every slot with itself, then for each prime, apply its factor to all its multiples:

```cpp
const int N = 1e6;
long long ph[N + 1];

void phiSieve() {
    for (int i = 0; i <= N; i++) ph[i] = i;
    for (int i = 2; i <= N; i++)
        if (ph[i] == i)                      // untouched ⟹ i is prime
            for (int j = i; j <= N; j += i)
                ph[j] -= ph[j] / i;
}
```

Tracing prime 2 across the array: `2 − 2/2 = 1`, `4 − 4/2 = 2`, `6 − 6/2 = 3`, `8 − 8/2 = 4`. Then prime 3 hits 6 again: `3 − 3/3 = 2`, giving the correct `φ(6) = 2`.

**O(N log log N)** for every value at once.

### Reference table

| n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|----|
| φ(n) | 1 | 1 | 2 | 2 | 4 | 2 | 6 | 4 | 6 | 4 |

---

## 12. Worked problem: sum of GCD over all pairs

> Compute `G(N) = Σ gcd(i, j)` over all pairs `1 ≤ i < j ≤ N`.

Brute force is O(N² log N). Here is the number-theoretic route.

### Step 1 — factor out the GCD

Suppose `gcd(i, j) = d`. Then `d` divides both, so write:

```
i = a·d        j = b·d
```

Substituting:

```
gcd(a·d, b·d) = d · gcd(a, b) = d      ⟹      gcd(a, b) = 1
```

**Pairs with GCD exactly `d` correspond one-to-one with coprime pairs `(a, b)`.** And since `i ≤ N`:

```
a·d ≤ N    ⟹    a ≤ N/d           likewise    b ≤ N/d
```

### Step 2 — count coprime pairs with the totient

We need the number of pairs `a < b ≤ N/d` with `gcd(a, b) = 1`. Fix `b` and count valid `a < b`: that is exactly the number of integers below `b` coprime to it — **φ(b)**. Summing over `b`:

```
C(d) = φ(2) + φ(3) + … + φ(N/d)
```

> **Convention:** the sum starts at `b = 2` because we want `a < b` strictly. Equivalently, treat `φ(1)` as 0 in the prefix sums for this problem. (The true value of φ(1) is 1; here it would wrongly count the pair (1,1).)

### Step 3 — assemble, with N = 10

| d | N/d | C(d) = φ(2) + … + φ(N/d) | value |
|---|-----|--------------------------|-------|
| 1 | 10 | 1+2+2+4+2+6+4+6+4 | 31 |
| 2 | 5 | 1+2+2+4 | 9 |
| 3 | 3 | 1+2 | 3 |
| 4 | 2 | 1 | 1 |
| 5 | 2 | 1 | 1 |
| 6..10 | 1 | (empty) | 0 |

Each pair contributes `d` to the sum:

```
G(10) = 1×31 + 2×9 + 3×3 + 4×1 + 5×1
      = 31 + 18 + 9 + 4 + 5
      = 67
```

### Step 4 — code

Prefix-sum the totients so each `C(d)` is a single lookup:

```cpp
long long sumOfGCDPairs(int n) {
    phiSieve();

    vector<long long> P(n + 1, 0);          // P[m] = phi(2) + ... + phi(m)
    for (int i = 2; i <= n; i++) P[i] = P[i-1] + ph[i];

    long long ans = 0;
    for (int d = 1; d <= n; d++)
        ans += (long long)d * P[n / d];
    return ans;
}
```

**O(N log log N)** for the sieve plus O(N) for the sum — down from O(N² log N). For `n = 10` this returns 67.

> The `n / d` pattern takes only about `2√n` distinct values, so this can be pushed to O(√n) per query with divisor-block decomposition if you need many queries.

---

## Appendix: bonus problem from the notes

> Count the primes of the form `P = x² + y⁴` with `P ≤ n`.

Bound each variable rather than testing every `P`:

```
y⁴ ≤ n    ⟹    y ≤ n^(1/4)
x² ≤ n − y⁴    ⟹    x ≤ √(n − y⁴)
```

For `n = 10⁷`: `y ≤ 56` and `x ≤ 3162`. That's roughly 10⁵ candidate pairs — trivial — versus 10⁷ if you scanned every value.

```cpp
sieve();                                  // isPrime up to n
long long cnt = 0;
for (long long y = 1; y * y * y * y <= n; y++) {
    long long y4 = y * y * y * y;
    for (long long x = 1; x * x <= n - y4; x++) {
        long long v = x * x + y4;
        if (isPrime[v]) cnt++;             // may double count; use a set if needed
    }
}
```

Two cautions: the same `P` can arise from more than one `(x, y)` pair, so deduplicate if the problem wants distinct primes; and the sieve must cover `n`, which caps this at around `n = 10⁷–10⁸`.

---

## Complexity summary

| Technique | Precompute | Per query |
|---|---|---|
| Trial division primality | — | O(√n) |
| Sieve of Eratosthenes | O(n log log n) | O(1) |
| Segmented sieve | O(√R log log √R) | O((R−L) log log R) |
| Factorize one number | — | O(√n) |
| SPF sieve | O(n log log n) | O(log n) |
| Divisor count | — | O(√n) |
| GCD (Euclid) | — | O(log min(a,b)) |
| Extended Euclid | — | O(log min(a,b)) |
| Binary exponentiation | — | O(log b) |
| Modular inverse (extgcd) | — | O(log m) |
| Modular inverse (Fermat) | — | O(log m) |
| nCr mod p | O(n) | O(1) |
| Inclusion–Exclusion | — | O(2^k × k) |
| φ(n) single | — | O(√n) |
| Totient sieve | O(n log log n) | O(1) |

---

## Notes on the source

Three small things I corrected or standardised while writing this up:

- **The `x² + y⁴` example.** The notes state `n = 10⁶` but then derive `y ≤ 57` and `x ≈ 3000`, which correspond to `n ≈ 10⁷`. I used 10⁷ so the numbers agree.
- **`φ(1)`.** The notes list `φ(1) = 0`. The standard value is 1; 0 is a deliberate convention for the pair-counting problem in §12, and I flagged it there rather than leaving it as a silent contradiction with the formula.
- **Sieve array bounds.** `vector<bool> Primes(1e6, true)` gives valid indices 0…999999, so the loop condition needs `< 1e6`, or the array needs size `N + 1`. I used `N + 1` throughout and made the inner loop bound match.

The syllabus lists **Segmented Sieve** (§3) and **Binomial Coefficients** (§9), but the notes only set up the constraints for the first and never reach the second. I wrote both sections out in full so the tutorial covers all nine listed topics.
