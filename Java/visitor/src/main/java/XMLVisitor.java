/**
 * Created by spoken on 22/11/16.
 */
public class XMLVisitor implements SumVisitor<String>, ProductVisitor<String>, VariableVisitor<String>, FormulaVisitor<String> {
    public String visit(Variable variable){ return variable.asString(); }
    public String visit(Sum sum){ return sum.asString(); }
    public String visit(Product product){ return product.asString(); }
}
