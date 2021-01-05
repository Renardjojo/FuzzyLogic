using System.Collections;
using System.Collections.Generic;
using FuzzyLogicPCL;
using FuzzyLogicPCL.FuzzySets;
using UnityEngine;

public class TestObject : MonoBehaviour
{
    public FuzzyLogic m_fuzzyLogic;
    
    // Start is called before the first frame update
    void Start()
    {
        // Création du système
        WriteLine("Gestion du zoom GPS", true);
        FuzzySystem system = new FuzzySystem("Gestion du zoom GPS");

        WriteLine("1) Ajout des variables", true);

        // Ajout de la variable linguistique "Distance" (de 0 à 500 000 m)
        WriteLine("Ajout de la variable Distance");
        LinguisticVariable distance = new LinguisticVariable("Distance", 0, 500000);
        distance.AddValue(new LinguisticValue("Faible", new LeftFuzzySet(0, 500000, 30, 50)));
        distance.AddValue(new LinguisticValue("Moyenne", new TrapezoidalFuzzySet(0, 500000, 40, 50, 100, 150)));
        distance.AddValue(new LinguisticValue("Grande", new RightFuzzySet(0, 500000, 100, 150)));
        system.addInputVariable(distance);

        // Ajout de la variable linguistique "Vitesse" (de 0 à 200)
        WriteLine("Ajout de la variable Vitesse");
        LinguisticVariable vitesse = new LinguisticVariable("Vitesse", 0, 200);
        vitesse.AddValue(new LinguisticValue("Lente", new LeftFuzzySet(0, 200, 20, 30)));
        vitesse.AddValue(new LinguisticValue("PeuRapide", new TrapezoidalFuzzySet(0, 200, 20, 30, 70, 80)));
        vitesse.AddValue(new LinguisticValue("Rapide", new TrapezoidalFuzzySet(0, 200, 70, 80, 90, 110)));
        vitesse.AddValue(new LinguisticValue("TresRapide", new RightFuzzySet(0, 200, 90, 110)));
        system.addInputVariable(vitesse);

        // Ajout de la variable linguistique "Zoom" (de 0 à 5)
        WriteLine("Ajout de la variable Zoom");
        LinguisticVariable zoom = new LinguisticVariable("Zoom", 0, 5);
        zoom.AddValue(new LinguisticValue("Petit", new LeftFuzzySet(0, 5, 1, 2)));
        zoom.AddValue(new LinguisticValue("Normal", new TrapezoidalFuzzySet(0, 5, 1, 2, 3, 4)));
        zoom.AddValue(new LinguisticValue("Gros", new RightFuzzySet(0, 5, 3, 4)));
        system.addOutputVariable(zoom);
     
        WriteLine("2) Ajout des règles", true);

        // Création des règles selon la matrice suivante :
        // Plus le zoom est petit, plus on voit loin (mais moins détaillé)
        // V \ D  || F | M | G |
        // Lent   || N | P | P |
        // Peu Ra || N | N | P |
        // Rapide || G | N | P |
        // Très R || G | G | P |
        system.addFuzzyRule("IF Distance IS Grande THEN Zoom IS Petit");
        system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS Lente THEN Zoom IS Normal");
        system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS PeuRapide THEN Zoom IS Normal");
        system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS Rapide THEN Zoom IS Gros");
        system.addFuzzyRule("IF Distance IS Faible AND Vitesse IS TresRapide THEN Zoom IS Gros");
        system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS Lente THEN Zoom IS Petit");
        system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS PeuRapide THEN Zoom IS Normal");
        system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS Rapide THEN Zoom IS Normal");
        system.addFuzzyRule("IF Distance IS Moyenne AND Vitesse IS TresRapide THEN Zoom IS Gros");
        WriteLine("9 règles ajoutées \n");

        WriteLine("3) Résolution de cas pratiques", true);
        // Cas pratique 1 : vitesse de 35 kms/h, et prochain changement de direction à 70m
        WriteLine("Cas 1 :", true);
        WriteLine("V = 35 (peu rapide)");
        WriteLine("D = 70 (moyenne)");
        system.SetInputVariable(vitesse, 35);
        system.SetInputVariable(distance, 70);
        WriteLine("Attendu : zoom normal, centroïde à 2.5");
        WriteLine("Résultat : " + system.Solve() + "\n");

        // Cas pratique 2 : vitesse de 25 kms/h, et prochain changement de direction à 70m
        system.ResetCase();
        WriteLine("Cas 2 :", true);
        WriteLine("V = 25 (50% lente, 50% peu rapide)");
        WriteLine("D = 70 (moyenne)");
        system.SetInputVariable(vitesse, 25);
        system.SetInputVariable(distance, 70);
        WriteLine("Attendu : zoom normal à 50% + zoom petit à 50%");
        WriteLine("Résultat : " + system.Solve() + "\n");

        // Cas pratique 3 : vitesse de 72.5 kms/h, et prochain changement de direction à 40m
        system.ResetCase(); 
        WriteLine("Cas 3 :", true);
        WriteLine("V = 72.5 (75% peu rapide + 25% rapide)");
        WriteLine("D = 40 (50% faible)");
        system.SetInputVariable(vitesse, 72.5);
        system.SetInputVariable(distance, 40);
        WriteLine("Attendu : zoom normal à 50% + zoom gros à 25%");
        WriteLine("Résultat : " + system.Solve() + "\n");
        
        // Cas pratique 4 : vitesse de 100 kms/h, et prochain changement de direction à 110m
        system.ResetCase();
        WriteLine("Cas 4 :", true);
        WriteLine("V = 100 (50% rapide + 50% très rapide)");
        WriteLine("D = 110 (80% moyenne, 20% grande)");
        system.SetInputVariable(vitesse, 100);
        system.SetInputVariable(distance, 110);
        WriteLine("Attendu : zoom petit à 20% + zoom normal à 50% + zoom gros à 50%");
        WriteLine("Résultat : " + system.Solve() + "\n");
        
        // Cas pratique 5 : vitesse de 45 kms/h, et prochain changement de direction à 160m
        system.ResetCase();
        WriteLine("Cas 5 :", true);
        WriteLine("V = 45 (100% peu rapide)");
        WriteLine("D = 160 (100% grande)");
        system.SetInputVariable(vitesse, 45);
        system.SetInputVariable(distance, 160);
        WriteLine("Attendu : zoom petit à 100%");
        WriteLine("Résultat : " + system.Solve() + "\n");
    }

        private static void WriteLine(string msg, bool stars = false)
        {
            if (stars)
            {
                msg = "*** " + msg + " ";
                while (msg.Length < 45)
                {
                    msg += "*";
                }
            }
            Debug.Log(msg);
        }
    // Update is called once per frame
    void Update()
    {
        
    }
}
