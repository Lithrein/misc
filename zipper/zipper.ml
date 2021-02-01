let rec times f x
= function
  | 1 -> f x
  | n -> f (times f x (n - 1))

let ( $ ) f x = f x
let ( << ) f g x = f (g x)

type 'a tree  = Hole | Node of 'a * 'a tree list
type 'a zip_l = 'a list * 'a list
type 'a zip_t = 'a tree * 'a tree zip_l

let start_l : 'a list -> 'a zip_l
= function
  | []      -> ([], [])
  | x :: xs -> (xs, [x])

let forward_l : 'a zip_l -> 'a zip_l
= function
  | (x :: [], l) -> (List.rev l, [x])
  | (x :: xs, l) -> (xs, x :: l)
  | z -> z

let backward_l : 'a zip_l -> 'a zip_l
= function
  | ([], x :: []) -> ([], [x])
  | (l,  x :: []) -> ([x], List.rev l)
  | (l,  x :: xs) -> (x :: l, xs)
  | z -> z

let get_l : 'a zip_l -> 'a
= function
  | (l, x :: xs) -> x
  | _ -> failwith "empty zipper"

let rec replace_hole_with r
= function
  | Hole -> r
  | Node(el, sons) ->
      Node(el, List.map (fun t -> replace_hole_with r t) sons)

let rec replace_hole_with_l r : 'a tree list -> 'a tree list
= function
  | [] -> []
  | Hole :: xs  -> r :: replace_hole_with_l r xs
  | Node(el, sons) :: xs -> Node(el, sons) :: replace_hole_with_l r xs

let rec without_hole_layer
  = function
    | Node(v, sons) ->
        if List.mem Hole sons
        then Hole
        else Node(v, List.map without_hole_layer sons)
    | Hole -> failwith ""

let rec hole_layer : 'a tree -> 'a * 'a tree list
= function
  | Node(v, sons) ->
      if List.mem Hole sons
      then (v, sons)
      else begin match List.map hole_layer sons with
           | [] -> (v,[])
           | x :: xs -> x
      end
  | Hole -> failwith ""

let start_t : 'a tree -> 'a zip_t
= fun t -> (Hole, start_l [t])

let up_t : 'a zip_t -> 'a zip_t
= function
  | (Hole, _)  -> failwith "can't go higher"
  | (t, (l, r)) ->
      let v, sons = hole_layer t in
      let new_sons = replace_hole_with_l (Node(v, (List.rev r) @ l)) sons in
      let new_t = without_hole_layer t in
      (new_t, start_l new_sons)

let down_t : 'a zip_t -> 'a zip_t
= function
  | (_ , ([], [])) -> failwith "can't go deeper"
  | (t , (l, (Node(el, sons)) :: r)) ->
      (replace_hole_with (Node(el, (List.rev r) @ (Hole :: l))) t, start_l sons)
  | _ -> failwith "panic"

let backward_t : 'a zip_t -> 'a zip_t
= function
  | (l, z) -> (l, backward_l z)

let forward_t : 'a zip_t -> 'a zip_t
= function
  | (l, z) -> (l, forward_l z)

let get_t : 'a zip_t -> 'a tree
= function
  | (_, (_, x :: xs)) -> x
  | _ -> failwith "nothing to get"

let teg_t : 'a zip_t -> 'a tree
= function
  | (t, _) -> t

let test =
  Node("Animaux",
    [ Node("Felins",
      [ Node("Lions",[])
      ; Node("Chats",[])
      ; Node("Panthere",[])])
    ; Node("Canins",
      [ Node("Chiens",[])
      ; Node("Loup",[])])
    ; Node("Tortues",
      [ Node("Tortue Galapagos",[])
      ; Node("Tortue Naine",[])])
    ])

let t = start_t test
