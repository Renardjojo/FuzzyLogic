/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-11 - 20 h 08
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "imgui.h"
#include "implot.h"
#include "AI/FuzzyLogic/LinguisticVariable.hpp"

namespace AI::FuzzyLogic::GUI
{
    template <typename TPrecisionType = float>
    void displayLinguistiqueVariable(const AI::FuzzyLogic::LinguisticVariable<TPrecisionType>& in_linguisticVariable)
    {
        bool show_lines = true;
        bool show_fills = true;

        static float fill_ref = 0;
        ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
        ImGui::Checkbox("Fills", &show_fills);

        ImPlot::SetNextPlotLimits(in_linguisticVariable.getMin(), in_linguisticVariable.getMax(), 0, 1);
        if (ImPlot::BeginPlot(in_linguisticVariable.getName().c_str(), in_linguisticVariable.getUnit().c_str(), "Degree of belonging"))
        {
            for (auto&& value : in_linguisticVariable.getValues())
            {
                std::vector<float> pointX;
                std::vector<float> pointY;

                for (auto&& point : value.getFuzzySet().getPoints())
                {
                    pointX.emplace_back(point.getX());
                    pointY.emplace_back(point.getY());
                }

                if (show_fills)
                {
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);

                        ImPlot::PlotShaded(value.getName().c_str(), pointX.data(), pointY.data(), pointX.size());

                    ImPlot::PopStyleVar();
                }
                if (show_lines)
                {
                    ImPlot::PlotLine(value.getName().c_str(), pointX.data(), pointY.data(), pointX.size());
                }
            }

            ImPlot::EndPlot();
        }
    }

    template <typename TPrecisionType = float>
    void displayLinguistiqueVariableWithCollapsingHeader(const AI::FuzzyLogic::LinguisticVariable<TPrecisionType>& in_linguisticVariable)
    {
        if (ImGui::CollapsingHeader(in_linguisticVariable.getName().c_str()))
        {
            displayLinguistiqueVariable<TPrecisionType>(in_linguisticVariable);
        }
    }

    template <typename TPrecisionType = float>
    void displayFuzzyRules(AI::FuzzyLogic::FuzzySystem<TPrecisionType>& in_system)
    {
        if (ImGui::BeginTable("##table1", in_system.getInputs()[0].getValues().size() + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            //Columns
            ImGui::TableSetupColumn((in_system.getInputs()[0].getName() + "/" + in_system.getInputs()[1].getName()).c_str());

            for (auto&& input : in_system.getInputs()[0].getValues())
            {
                float font_size = ImGui::GetFontSize() * input.getName().size() / 2;
                ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
                ImGui::TableSetupColumn(input.getName().c_str());
            }

            //Row
            ImGui::TableHeadersRow();

            for (int row = 0; row < in_system.getInputs()[1].getValues().size(); ++row)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);

                ImGui::Text(in_system.getInputs()[1].getValues()[row].getName().c_str());

                int idElem = 0;

                for (int column = 1; column < in_system.getInputs()[0].getValues().size() + 1; ++column)
                {
                    ImGui::TableSetColumnIndex(column);

                    idElem = row * in_system.getInputs()[0].getValues().size() + (column - 1);
                    if (ImGui::Button((in_system.getRules()[idElem].getConclusion().getName() + "##" + std::to_string(idElem)).c_str(), ImVec2(-FLT_MIN, 0.0f)))
                    {
                        int currentIndex = 0;
                        while (in_system.getOutput().getValues()[currentIndex].getName() != in_system.getRules()[idElem].getConclusion().getName().c_str())
                        {
                            ++currentIndex;
                        }

                        in_system.getRules()[idElem].setConclusion(FuzzyExpression(in_system.getOutput(), in_system.getOutput().getValues()[++currentIndex % in_system.getOutput().getValues().size()].getName()));
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

} /*namespace AI::FuzzyLogic::GUI*/
