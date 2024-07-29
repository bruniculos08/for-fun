-- Fazendo as funções do algoritmo de Tonelli-Shanks com mônada para contar o número de iterações:
data Counter a = Ct (a, Int) deriving Show

instance Functor Counter where
    fmap f (Ct (a, n)) = Ct (f a, n + 1)

instance Applicative Counter where
    Ct (f, n1) <*> Ct (a, n2) = Ct (f a, n1 + n2 + 1)
    pure x = Ct (x, 0)

instance Monad Counter where
    Ct (x, n1) >>= f = let Ct (y, n2) = f x in Ct (y, n1 + n2)

powMod b e m = powMod' 0 1
                where
                powMod' e' c = if e' < e then do powMod' (e' + 1) (mod  (b * c) m)
                                else Ct (c, 1)

factorTwoPower x | even x = do
                            (_, acc) <- factorTwoPower (div x 2)
                            return (div x (2^(acc + 1)), 1 + acc)
                | otherwise = pure (x, 0)


legendre a p = let Ct (v, n) = powMod a (div (p-1) 2) p in
                if v == mod (-1) p then Ct (-1, n + 1)
                else Ct (v, n + 1)

nonResidue' n p = if n >= p then Ct (Nothing, 1) else
                    do
                    x <- legendre n p
                    if x == -1 then
                        return $ Just n
                    else
                        do
                            nonResidue' (x + 1) p

nonResidue = nonResidue' 1

powSquaring b e m
    | e == 0 = Ct (1, 1)
    | even e = let Ct (a, n) = powSquaring b (div e 2) m in Ct (mod (a^2) m, n + 1)
    | otherwise =  let Ct (a, n) = powSquaring b (div (e - 1) 2) m in Ct (mod (b * (a^2)) m, n + 1)

repSquaring t p = repSquaring' 0
                    where
                        repSquaring' i = do
                                        r <- powSquaring t (2^i) p
                                        if r == 1 then return i
                                        else do
                                                repSquaring' (i + 1)

loop :: Int -> Int -> Int -> Int -> Int -> Counter (Maybe Int)
loop m c t r p | mod t p == 0 = Ct (Nothing, 1)
               | mod t p == 1 = Ct (Just r, 1)
               | otherwise = do
                                i <- repSquaring t p
                                if m == i then return Nothing
                                else do
                                        b <- powSquaring c (2^(m - i - 1)) p
                                        b_squered <- powMod b 2 p
                                        loop i b_squered (mod (t * b_squered) p) (mod (r * b) p) p


tonelli n p =
            do
            qs <- factorTwoPower (p - 1)
            let (q, s) = qs
            result_of_z <- nonResidue p
            case result_of_z of
                Nothing -> return Nothing
                Just z -> do
                            c <- powMod z q p
                            t <- powMod n q p
                            r <- powMod n (div (q + 1) 2) p
                            loop s c t r p


tonelli' n p =  
        factorTwoPower (p - 1) >>= \qs -> let (q, s) = qs in nonResidue p >>= \result_of_z ->
        case
        result_of_z of
            Nothing -> return Nothing
            Just z -> powMod z q p >>= \c -> powMod n q p >>= \t -> 
                powMod n (div (q + 1) 2) p >>= \r -> loop s c t r p 
         