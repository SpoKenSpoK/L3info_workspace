public abstract class VariadicOperator implements Formula {
    private Formula[] formulas;

    public VariadicOperator(Formula ... formulas){
        this.formulas = formulas;
    }

    public double asValue(){
        double acc = initialValue();
        for(Formula f : formulas)
            acc = cumulativeValue(acc, f.asValue());
        return acc;
    }

    public String asString(){
        StringBuilder sb = new StringBuilder("(");
        for(int i=0; i<formulas.length; ++i){
            if(i!=0) sb.append( symbol());
            sb.append(formulas[i].asString());
        }
        sb.append(")");
        return sb.toString();
    }

    protected abstract double initialValue();
    protected abstract String symbol();
    protected abstract double cumulativeValue(double accumulator, double value);

}

