// Instanciation apparemment moins agréable car il faut instancier la stratégie

public class Main {
    public static void main(String argv[]){
        Formula x = new Variable("x",3);
        Formula y = new Variable("y",6);
        Formula z = new Variable("z",9);

        Formula f = new VariadicOperator(new Sum(), x, y, z); // x y et z sont disposés automatiquement dans un tableau

        // Sum n'a pas d'état, on pourrait partager le même exemplaire d'instance.
        /* Modification possible :
            public class Sum ... {
                    public static final instance = new Sum();
            }

            // Ce qui donne :
            Formula f = new VariadicOperator(Sum.instance, x, y, z);
            Formula g = new VariadicOperator(Sum.instance, x, f);
            // => reste lourd POV ecriture

            Modification possible n°2 :
            Ajouter une factory method statique dans chaque strategie :

            public class Sum ... {
                public static VariadicOperator variadic(Formula ... formulas){
                    return new VariadicOperator(instance, formulas);
                }
            }

            On peut maintenant écrire :
            Formula f = Sum.variatic(x, y, z);
            Formula g = Sum.variatic(x, f);

            => invocation de la factory method
        */
    }
}
