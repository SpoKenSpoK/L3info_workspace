public class VariadicOperator implements Formula { // Classe concrete
    private Formula[] formulas;
    private Operator operator;
    public VariadicOperator(Operator operator, Formula ... formulas){
        this.operator = operator;
        this.formulas = formulas;
    } // méthode variadique

    @Override
    public double asValue(){
        double acc = operator.initialValue();
        for(Formula f : formulas)
            acc = operator.cumulativeValue(acc, f.asValue()); // Délégation à la STRATEGIE (Parce que design Pattern STRATEGY TAVU)
        return acc;
    }

    @Override
    public String asString(){
        StringBuilder sb = new StringBuilder("(");
        for(int i=0; i<formulas.length; ++i){
            if(i!=0) sb.append(operator.symbol()); // Délégation à la stratégie
            sb.append(formulas[i].asString());
        }
        sb.append(")");
        return sb.toString();
    }
}
