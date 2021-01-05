using FuzzyLogicPCL.FuzzySets;
using System;
using System.Collections.Generic;

namespace FuzzyLogicPCL
{
    public class LinguisticVariable
    {
        internal String Name { get; set; }
        List<LinguisticValue> Values { get; set; }
        internal Double MinValue { get; set; }
        internal Double MaxValue { get; set; }

        public LinguisticVariable(String _name, double _min, double _max)
        {
            Values = new List<LinguisticValue>();
            Name = _name;
            MinValue = _min;
            MaxValue = _max;
        }

        public void AddValue(LinguisticValue lv) {
            Values.Add(lv);
        }

        public void AddValue(String name, FuzzySet fs)
        {
            Values.Add(new LinguisticValue(name, fs));
        }

        public void ClearValues() {
            Values.Clear();
        }

        internal LinguisticValue LinguisticValueByName(string name)
        {
            name = name.ToUpper();
            foreach (LinguisticValue val in Values)
            {
                if (val.Name.ToUpper().Equals(name))
                {
                    return val;
                }
            }
            return null;
        }
    }
}
