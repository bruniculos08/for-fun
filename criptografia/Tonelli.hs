import Data.List

powMod b e m = powMod' 0 1
                where
                powMod' e' c
                    | e' < e = powMod' (e' + 1) (mod  (b * c) m)
                    | otherwise = c

factorTwoPower :: Int -> (Int, Int)
factorTwoPower n | even n = let acc = snd (factorTwoPower (div n 2)) in (div n (2^(acc + 1)), 1 + acc)
                 | otherwise = (n, 0)

legendre :: Int -> Int -> Int
legendre a p | powMod a (div (p-1) 2) p == mod (-1) p = -1
             | otherwise = powMod a (div (p-1) 2) p

nonResidue' n p | n >= p = Nothing
                | otherwise = if legendre n p == -1 then Just n else nonResidue' (n + 1) p

nonResidue = nonResidue' 1

powSquaring b e m
    | e == 0 = 1
    | even e = let a = powSquaring b (div e 2) m in mod (a^2) m
    | otherwise =  let a = powSquaring b (div (e - 1) 2) m in mod (b * (a^2)) m

repSquaring :: Int -> Int -> Int
repSquaring t p = repSquaring' 0
                    where
                        repSquaring' i
                            | powSquaring t (2^i) p == 1 = i
                            | otherwise = repSquaring' (i + 1)

loop m c t r p | mod t p == 0 = Just 0
               | mod t p == 1 = Just r
               | otherwise = let i = repSquaring t p in
                    if m == i then Nothing else
                    let b = powSquaring c (2^(m - i - 1)) p in loop i (powMod b 2 p) (mod (t * powMod b 2 p) p) (mod (r * b) p) p

tonelli n p =
            do
            let (q, s) = factorTwoPower (p - 1)
            let (Just z) = nonResidue p
            let m = s
            let c = powMod z q p
            let t = powMod n q p
            let r = powMod n (div (q + 1) 2) p
            putStr (show m ++ ", " ++ show c ++ ", " ++ show t ++ ", " ++ show r ++ "\n")
            return (loop m c t r p)
