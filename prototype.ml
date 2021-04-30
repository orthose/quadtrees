(************************************************)
(* TP ALGO L3 Université Paris-Saclay 2020-2021 *)
(* Étudiant: Maxime Vincent                     *)
(* Prototypage de fonctions en OCaml            *)
(* Exécution: ocaml prototype.ml                *)
(************************************************)

(* Exercice 2 *)

(* SousEnsemble *)
let rec se l1 l2 =
    match (l1, l2) with
    (* Cas de base *)
    | ([], _) -> true
    | (_, []) -> false
    (* Élimination des doublons *)
    | (a::b::c, _) when a = b -> se (b::c) l2
    | (_, a::b::c) when a = b -> se l1 (b::c)
    (* Cas général *)
    | (a::b, c::d) ->
        if a > c then se l1 d
        else if a < c then false
        else se b d (* a = c *)

(* Diviser pour régner Permutations *)
let rec aettl x l =
    match l with
    | [] -> []
    | a::b -> (x::a)::(aettl x b)

let rec atp x l =
    match l with
    | [] -> [[x]]
    | y::s -> (x::l)::(aettl y (atp x s))
    
let rec atltp x l =
    match l with
    | [] -> []
    | a::b -> (atp x a)@(atltp x b)
    
let rec perm n =
    if n = 0 then [[]]
    else atltp n (perm (n-1))

(* SommeAvantApres *)
let sap l =
    let rec sap l before after =
        match l with
        | [] -> false
        | x::s -> 
            let r = sap s (x + before) after in
            if before = !after then true
            else let () = after := !after + x in r
    in
    let a = ref 0 in
    sap l 0 a
    
(* Exercice 3 *)

(* Arbre binaire *)
type a = F | N of a * a

(* CompetSousImagesPleines pour arbre binaire *)
let csip a h =
    let rec csip a =
        match a with
        | F -> (1, 0)
        | N(g, d) ->
            let (n, h1) = csip g in
            let (m, h2) = csip d in
            if h1 = h && h2 = h then (n + m, h1)
            else if h1 = h then (n, h1)
            else if h2 = h then (m, h2)
            else if n = m && h1 = h2 then (1, h1 + 1)
            else (0, h)
    in
    let (n, fh) = csip a in
    if fh = h then n else 0

(* Instance d'arbre binaire *)
let arbre = 
    N(
        N(
            N(
                N(F, F),
                N(F, F)
            ), 
            F
        ),
        N(
            N(
               N(F, F),
               N(F, F) 
            ),
            F
        )
    )

(* Quadtrees *)
type qt = B | N | C of qt * qt * qt * qt

(* compteSousImagesPleines quadtrees version fonctionnelle *)
let csip i h =
    let rec csip i =
        match i with
        | B | N -> (1, 0)
        | C(i1, i2, i3, i4) ->
            (* On parcourt l'arbre jusqu'en bas *)
            let l = [csip i1; csip i2; csip i3; csip i4] in
            let n1 = fst (List.hd l) in
            let h1 = snd (List.hd l) in 
            (* On fait remonter le nombre de sous-images pleines de hauteur h *)
            if List.exists (fun (_, h') -> h = h') l then
                ((List.fold_left (fun acc (n, h') -> if h = h' then acc + n else acc) 0 l), h)
            (* Si l'image est composée de sous-images pleines de hauteur h'
            alors c'est une image pleine de hauteur h' + 1 *)
            else if List.for_all (fun (n, h') -> n1 = n && h1 = h') l then (1, h1 + 1)
            (* L'image n'est pas une image pleine de hauteur h *)
            else (0, h)
    in
    let (n, fh) = csip i in
    (* Il est possible d'avoir trouvé des sous-images pleines 
    de hauteur inférieure à h *)
    if fh = h then n else 0

(* Instances de quadtrees *)
let i1 = 
    C(
        C(B, B, N, B),
        C(N, N, B, N),
        C(B, B, B, N),
        C(N, N, N, B)
    )
    
let i2 =
    C(
        C(
            C(B, B, N, B),
            C(N, N, B, N),
            C(B, B, B, N),
            C(N, N, N, B)
        ),
        N,
        C(
            N, B, N, 
            C(B, B, N, B)
        ),
        C(
            B, N, B,
            C(
                C(B, B, N, B),
                C(N, B, B, N),
                C(B, N, B, N),
                C(N, B, N, B)
            )
        )
    )

(* Version impérative *)
let csip i h =
    let rec csip i n h'=
        match i with
        | B | N -> let () = n := 1 in let () = h' := 0 in ()
        | C(i1, i2, i3, i4) ->
            (* On parcourt l'arbre jusqu'en bas *)
            let () = csip i1 n h' in let n1 = !n in let h1 = !h' in
            let () = csip i2 n h' in let n2 = !n in let h2 = !h' in
            let () = csip i3 n h' in let n3 = !n in let h3 = !h' in
            let () = csip i4 n h' in let n4 = !n in let h4 = !h' in
            let l = [(n1, h1); (n2, h2); (n3, h3); (n4, h4)] in
            (* On fait remonter le nombre de sous-images pleines de hauteur h *)
            if List.exists (fun (_, h') -> h = h') l then
                let () = n := (List.fold_left (fun acc (n, h') -> if h = h' then acc + n else acc) 0 l) in
                let () = h' := h in ()
            (* Si l'image est composée de sous-images pleines de hauteur h'
            alors c'est une image pleine de hauteur h' + 1 *)
            else if List.for_all (fun (n, h') -> n1 = n && h1 = h') l then
                let () = n := 1 in
                let () = h' := h1 + 1 in ()
            (* L'image n'est pas une image pleine de hauteur h *)
            else
                let () = n := 0 in
                let () = h' := h in ()  
    in
    let n = ref 0 in
    let h' = ref 0 in
    let () = csip i n h' in
    (* Il est possible d'avoir trouvé des sous-images pleines 
    de hauteur inférieure à h *)
    if !h' = h then !n else 0 
    
(* Aire d'une quadtree sans sous-fonction*)
let rec aire i =
    match i with
    | B -> 0.
    | N -> 1.
    | C(a,b,c,d) ->
        ((aire a) /. 4.) +. 
        ((aire b) /. 4.) +. 
        ((aire c) /. 4.) +. 
        ((aire d) /. 4.)   

(* Simplifie version fonctionnelle *)
let rec simpl i =
    match i with
    | B -> B
    | N -> N
    | C(a, b, c, d) ->
        let res = C(simpl a, simpl b, simpl c, simpl d) in
        match res with
        | C(B, B, B, B) -> B
        | C(N, N, N, N) -> N
        | _ -> res
          
let i3 = 
    C(
        C(N, N, C(N, N, N, C(N, N, N, C(N, N, N, N))), N),
        C(B, B, B, B), 
        N,
        C(B, N, B, N)
    ) 
    
let rec pow x n = 
    if n = 0 then 1 
    else if n > 0 then x * (pow x (n - 1))
    else raise (Invalid_argument "pow: n < 0")
 
(* Affichage 2k pixels version impérative *)
let print_matrice i p =
  let dim = pow 2 p in
  
  let write m b ofi ofj cote =
    let c = if b then '.' else '8' in
    for i = ofi to ofi + cote - 1 do
      for j = ofj to ofj + cote - 1 do
        m.(i).(j) <- c
      done
    done
  in
  
  let rec fill i m ofi ofj cote =
    match i with
    | B -> write m true ofi ofj cote
    | N -> write m false ofi ofj cote
    | C(a, b, c, d) ->
        if cote / 2 < 1 then m.(ofi).(ofj) <- '-'
        else 
          fill a m ofi ofj (cote / 2);
          fill b m ofi (ofj + cote / 2) (cote / 2);
          fill c m (ofi + cote / 2) ofj (cote / 2);
          fill d m (ofi + cote / 2) (ofj + cote / 2) (cote / 2)
  in
  
  let m = Array.make_matrix dim dim '.' in
  let () = fill i m 0 0 dim in
  for i = 0 to dim - 1 do
    for j = 0 to dim - 1 do
      print_char m.(i).(j)
    done;
    print_char '\n'
  done
  
(* Version sans tableau *)
let print_matrice i p =
    let dim = pow 2 p in
    
    let rec find_char i x y cote =
        match i with
        | B -> '.' | N -> '8'
        | C(a, b, c, d) ->
            let cote_div_2 = cote / 2 in
            if cote_div_2 < 1 then '-'
            else if x < cote_div_2 && y < cote_div_2 then 
                find_char a x y cote_div_2
            else if cote_div_2 <= x && y < cote_div_2 then
                find_char b (x - cote_div_2) y cote_div_2
            else if x < cote_div_2 && cote_div_2 <= y then
                find_char c x (y - cote_div_2) cote_div_2
            else 
                find_char d (x - cote_div_2) (y - cote_div_2) cote_div_2
    in            
        
    for y = 0 to dim - 1 do
        for x = 0 to dim - 1 do
            print_char (find_char i x y dim)
        done;
        print_char '\n'
    done   
    
(* Alea version impérative *)
let alea p n =
  let rec alea k n m =
    if k = p then (
      if (float_of_int !n /. float_of_int !m) >= Random.float 1.0 then (
        n := !n - 1; m := !m - 1; N 
      )
      else (
        m := !m - 1; B
      )
    )
    else
      C(alea (k+1) n m, alea (k+1) n m, alea (k+1) n m, alea (k+1) n m)
  in
  let dim = 2.** float_of_int p in
  alea 0 (ref n) (ref (int_of_float (dim *. dim)))
