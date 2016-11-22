public class Sum extends VariadicOperator {
    public Sum(Formula ... formulas){ super(formulas); }
	public String symbol(){ return "+"; }
    public double initialValue(){ return 0.0; }
    public double cumulativeValue(double acc, double value){ return (acc + value); }
    public <R> R accept(FormulaVisitor<R> formulaVisitor) {
        return formulaVisitor.visit(this);
    }
}
