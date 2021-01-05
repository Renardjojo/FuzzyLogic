using FuzzyLogicPCL.FuzzySets;
using System;
using System.Collections.Generic;

namespace FuzzyLogicPCL
{
    public class FuzzyRule
    {
        List<FuzzyExpression> Premises;
        FuzzyExpression Conclusion;

        public FuzzyRule(List<FuzzyExpression> _prem, FuzzyExpression _concl)
        {
            Premises = _prem;
            Conclusion = _concl;
        }

        private LinguisticValue val;
        private FuzzyExpression rulePremise;
        private FuzzyValue problemValue;
        private double ruleDegree;

        internal FuzzySet Apply(List<FuzzyValue> Problem)
        {
            ruleDegree = 1;
            foreach (FuzzyExpression localPremise in Premises)
            {
                rulePremise = localPremise;
                double localDegree = SearchAndComputePremiseDegree(Problem);
                if (LinguisticValueNotFound())
                {
                    return null;
                }
                ChangeOverallRuleDegree(localDegree);
            }
            return ComputeResultingFuzzySet();
        }

        private double SearchAndComputePremiseDegree(List<FuzzyValue> Problem)
        {
            val = null;
            double premiseDegree = 0;
            List<FuzzyValue>.Enumerator enumerator = Problem.GetEnumerator();
            bool valueFound = false;
            while(enumerator.MoveNext() && !valueFound)
            {
                problemValue = enumerator.Current;
                if (rulePremise.Lv == problemValue.Lv)
                {
                    premiseDegree = ComputeDegree();
                    valueFound = true;
                }
            }
            return premiseDegree;
        }

        private double ComputeDegree()
        {
            val = rulePremise.Lv.LinguisticValueByName(rulePremise.LinguisticValueName);
            if (val != null)
            {
                return val.DegreeAtValue(problemValue.Value); // this is fuzzyfication here
            }
            else
            {
                return double.NaN;
            }
        }

        private bool LinguisticValueNotFound()
        {
            return val == null;
        }

        private void ChangeOverallRuleDegree(double localDegree)
        {
            ruleDegree = Math.Min(ruleDegree, localDegree);
        }

        private FuzzySet ComputeResultingFuzzySet()
        {
            return Conclusion.Lv.LinguisticValueByName(Conclusion.LinguisticValueName).Fs * ruleDegree;
        }

        public FuzzyRule(string ruleStr, FuzzySystem fuzzySystem)
        {
            ruleStr = ruleStr.ToUpper();

            // Split premises and conclusion
            String[] rule = ruleStr.Split(new String[]{" THEN "}, StringSplitOptions.RemoveEmptyEntries);
            if (rule.Length == 2)
            {
                // Compute and add premises
                rule[0] = rule[0].Remove(0, 2); // On enlève "IF"
                String[] prem = rule[0].Trim().Split(new String[] {" AND "}, StringSplitOptions.RemoveEmptyEntries);
                Premises = new List<FuzzyExpression>();
                foreach (String exp in prem)
                {
                    String[] res = exp.Split(new String[] { " IS " }, StringSplitOptions.RemoveEmptyEntries);
                    if (res.Length == 2)
                    {
                        FuzzyExpression fexp = new FuzzyExpression(fuzzySystem.LinguisticVariableByName(res[0]), res[1]);
                        Premises.Add(fexp);
                    }
                }
                // Add conclusion
                String[] conclu = rule[1].Split(new String[] {" IS "}, StringSplitOptions.RemoveEmptyEntries);
                if (conclu.Length == 2)
                {
                    Conclusion = new FuzzyExpression(fuzzySystem.LinguisticVariableByName(conclu[0]), conclu[1]);
                }
            }
        }
    }
}
