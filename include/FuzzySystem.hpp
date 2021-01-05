using FuzzyLogicPCL.FuzzySets;
using System;
using System.Collections.Generic;

namespace FuzzyLogicPCL
{
    public class FuzzySystem
    {
        String Name { get; set; }
        List<LinguisticVariable> Inputs;
        LinguisticVariable Output;
        List<FuzzyRule> Rules;
        List<FuzzyValue> Problem;

        public FuzzySystem(String _name)
        {
            Name = _name;
            Inputs = new List<LinguisticVariable>();
            Rules = new List<FuzzyRule>();
            Problem = new List<FuzzyValue>();
        }

        public void addInputVariable(LinguisticVariable lv)
        {
            Inputs.Add(lv);
        }

        public void addOutputVariable(LinguisticVariable lv)
        {
            Output = lv;
        }

        public void addFuzzyRule(FuzzyRule fuzzyRule)
        {
            Rules.Add(fuzzyRule);
        }

        public void addFuzzyRule(string ruleStr)
        {
            FuzzyRule rule = new FuzzyRule(ruleStr, this);
            Rules.Add(rule);
        }

        public void SetInputVariable(LinguisticVariable inputVar, double value)
        {
            Problem.Add(new FuzzyValue(inputVar, value));
        }
        
        public void ResetCase()
        {
            Problem.Clear();
        }

        internal LinguisticVariable LinguisticVariableByName(string name)
        {
            foreach (LinguisticVariable input in Inputs)
            {
                if (input.Name.ToUpper().Equals(name))
                {
                    return input;
                }
            }
            if (Output.Name.ToUpper().Equals(name))
            {
                return Output;
            }
            return null;
        }
        
        public double Solve()
        {
            FuzzySet res = new FuzzySet(Output.MinValue, Output.MaxValue);
            res.Add(Output.MinValue, 0);
            res.Add(Output.MaxValue, 0);
            foreach (FuzzyRule rule in Rules)
            {
                FuzzySet resultingSet = rule.Apply(Problem);
                if (!FuzzySet.ReferenceEquals(resultingSet, null))
                {
                    res = res | resultingSet;
                }
            }
            return res.Centroid();
        }
    }
}
