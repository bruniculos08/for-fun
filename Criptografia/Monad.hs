-- Fazendo as funções do algoritmo de Tonelli-Shanks com mônada para contar o número de iterações:
data Counter a = Ct (a, Int)

instance Functor Counter where
    fmap f (Ct (a, n)) = Ct (f a, n + 1)

instance Applicative Counter where
    Ct (f, n1) <*> Ct (a, n2) = Ct (f a, n1 + n2 + 1)
    pure x = Ct (x, 0)

instance Monad Counter where
    Ct (x, n1) >>= f = let Ct (y, n2) = f x in Ct (y, n1 + n2)

powMod b e m = powMod' 0 1
                where
                powMod' e' c
                    | e' < e = let Ct (x, n) = powMod' (e' + 1) (mod  (b * c) m) in Ct (x, n + 1)
                    | otherwise = Ct (c, 1)
