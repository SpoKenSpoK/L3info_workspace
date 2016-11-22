public class Product extends VariadicOperator {
    public Product(Formula ... formulas){ super(formulas); }
	public String symbol(){ return "*"; }
    public double initialValue(){ return 1.0; }
    public double cumulativeValue(double acc, double value){ return (acc * value); }

    public <R> R accept(FormulaVisitor<R> formulaVisitor) {
        return formulaVisitor.visit(this);
    }
}
