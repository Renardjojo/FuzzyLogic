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
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "AI/FuzzyLogic/FuzzyLogic.hpp"

namespace AI::FuzzyLogic::GUI
{
    template <typename TPrecisionType = float>
    void displayFuzzysificationAndDeffuzification(const AI::FuzzyLogic::FuzzySet::FuzzySet<TPrecisionType>& in_fuzzySet, TPrecisionType centroid, const std::string& in_graphName, const std::string& in_unit)
    {
        bool show_lines = true;
        bool show_fills = true;

        static float fill_ref = 0;
        ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
        ImGui::Checkbox("Fills", &show_fills);

        ImPlot::SetNextPlotLimits(in_fuzzySet.getMin(), in_fuzzySet.getMax(), 0, 1);
        if (ImPlot::BeginPlot(in_graphName.c_str(), in_unit.c_str(), "Degree of belonging"))
        {
            std::vector<float> pointX;
            std::vector<float> pointY;

            for (auto&& point : in_fuzzySet.getPoints())
            {
                pointX.emplace_back(point.getX());
                pointY.emplace_back(point.getY());
            }

            if (show_fills)
            {
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);

                ImPlot::PlotShaded("Fuzzification", pointX.data(), pointY.data(), pointX.size());

                ImPlot::PopStyleVar();
            }
            if (show_lines)
            {
                ImPlot::PlotLine("Fuzzification", pointX.data(), pointY.data(), pointX.size());
            }
        }

        float xs[2] = { centroid, centroid };
        float ys[2] = { 0, 1 };

        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, ImPlot::GetStyle().MarkerSize, ImVec4(0, 0, 0, 0), ImPlot::GetStyle().MarkerWeight);
        ImPlot::PlotLine("##CentroidMarker", xs, ys, 2);

        ImPlot::EndPlot();
    }

    template <typename TPrecisionType = float>
    void displayLinguisticVariable(const AI::FuzzyLogic::LinguisticVariable<TPrecisionType>& in_linguisticVariable)
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
        }
        ImPlot::EndPlot();
    }

    template <typename TPrecisionType = float>
    void displayLinguisticVariableWithCollapsingHeader(const AI::FuzzyLogic::LinguisticVariable<TPrecisionType>& in_linguisticVariable)
    {
        if (ImGui::CollapsingHeader(in_linguisticVariable.getName().c_str()))
        {
            displayLinguisticVariable<TPrecisionType>(in_linguisticVariable);
        }
    }

    template <typename TPrecisionType = float>
    void displayEditableLinguisticVariable(AI::FuzzyLogic::LinguisticVariable<TPrecisionType>& in_linguisticVariable)
    {
        ImGui::PushID(&in_linguisticVariable);

        if (ImGui::CollapsingHeader((in_linguisticVariable.getName() + "###CollapsingHeader").c_str()))
        {
            ImGui::InputText("Name", &in_linguisticVariable.getName());
            ImGui::DragFloatRange2("Min/Max values", &in_linguisticVariable.getMin(), &in_linguisticVariable.getMax());

            bool show_lines = true;
            bool show_fills = true;

            static float fill_ref = 0;
            ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
            ImGui::Checkbox("Fills", &show_fills);

            ImGui::BeginGroup();

            // allow labels to be dragged and dropped
            ImGui::Selectable("Left", false, 0, ImVec2(100, 0));
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                FuzzySet::LeftFuzzySet<TPrecisionType> payloadData(-1.f, 1.f, -0.5f, 0.5f);
                ImGui::SetDragDropPayload("DND_PLOT", payloadData.getPoints().data(), payloadData.getPoints().size() * sizeof(FuzzySet::Point2D<TPrecisionType>));
                ImGui::TextUnformatted("Left");
                ImGui::EndDragDropSource();
            }

            ImGui::Selectable("Trapezoidal", false, 0, ImVec2(100, 0));
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                FuzzySet::TrapezoidalFuzzySet<TPrecisionType> payloadData(-1.f, 1.f, -1.f, -0.5f, 0.5f, 1.f);
                ImGui::SetDragDropPayload("DND_PLOT", payloadData.getPoints().data(), payloadData.getPoints().size() * sizeof(FuzzySet::Point2D<TPrecisionType>));
                ImGui::TextUnformatted("Trapezoidal");
                ImGui::EndDragDropSource();
            }

            ImGui::Selectable("Triangular", false, 0, ImVec2(100, 0));
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                FuzzySet::TriangularFuzzySet<TPrecisionType> payloadData(-1.f, 1.f, -1.f, 0.f, 1.f);
                ImGui::SetDragDropPayload("DND_PLOT", payloadData.getPoints().data(), payloadData.getPoints().size() * sizeof(FuzzySet::Point2D<TPrecisionType>));
                ImGui::TextUnformatted("Triangular");
                ImGui::EndDragDropSource();
            }

            ImGui::Selectable("Right", false, 0, ImVec2(100, 0));
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                FuzzySet::RightFuzzySet<TPrecisionType> payloadData(-1.f, 1.f, -0.5f, 0.5f);
                ImGui::SetDragDropPayload("DND_PLOT", payloadData.getPoints().data(), payloadData.getPoints().size() * sizeof(FuzzySet::Point2D<TPrecisionType>));
                ImGui::TextUnformatted("Right");
                ImGui::EndDragDropSource();
            }

            ImGui::EndGroup();
            ImGui::SameLine();

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

                //Start drag and drop prefab fuzzy set
                // make our plot a drag and drop target
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PLOT"))
                    {
                        std::vector<FuzzySet::Point2D<TPrecisionType>> points(static_cast<FuzzySet::Point2D<TPrecisionType>*>(payload->Data), static_cast<FuzzySet::Point2D<TPrecisionType>*>(payload->Data) + payload->DataSize / sizeof(FuzzySet::Point2D<TPrecisionType>));
                        ImPlotPoint mousePos = ImPlot::GetPlotMousePos();

                        for (FuzzySet::Point2D<TPrecisionType>& point : points)
                        {
                            point.setX(point.getX() + mousePos.x);
                        }

                        FuzzySet::FuzzySet<TPrecisionType> newFuzzySet(points.front().getX(), points.back().getX());
                        newFuzzySet.setPoints(points);

                        std::string nameId;
                        int cpId = 0;
                        bool valueFound;

                        do
                        {
                            valueFound = false;
                            nameId = cpId == 0 ? "Unknow" : "Unknow" + std::to_string(cpId);

                            for (auto&& value : in_linguisticVariable.getValues())
                            {
                                if (value.getName() == nameId)
                                {
                                    valueFound = true;
                                    ++cpId;
                                    break;
                                }
                            }

                        } while (valueFound);


                        in_linguisticVariable.addValue(LinguisticValue<TPrecisionType>(nameId, newFuzzySet));
                    }
                    ImGui::EndDragDropTarget();
                }

                static float  frequency = 0.1f;
                static float  amplitude = 0.5f;
                static ImVec4 color = ImVec4(1, 1, 0, 1);
                static float  alpha = 1.0f;
                static bool   line = false;
                static float  thickness = 1;
                static bool   markers = false;
                static bool   shaded = false;

                if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(1))
                {
                    ImGui::OpenPopup("my_toggle_popup");
                }

                if (ImGui::BeginPopup("my_toggle_popup"))
                {
                    ImGui::SliderFloat("Frequency", &frequency, 0, 1, "%0.2f");
                    ImGui::SliderFloat("Amplitude", &amplitude, 0, 1, "%0.2f");
                    ImGui::Separator();
                    ImGui::ColorEdit3("Color", &color.x);
                    ImGui::SliderFloat("Transparency", &alpha, 0, 1, "%.2f");
                    ImGui::Checkbox("Line Plot", &line);
                    if (line)
                    {
                        ImGui::SliderFloat("Thickness", &thickness, 0, 5);
                        ImGui::Checkbox("Markers", &markers);
                        ImGui::Checkbox("Shaded", &shaded);
                    }
                    ImGui::EndPopup();
                }
            }
            ImPlot::EndPlot();
        }

        ImGui::PopID();
    }

    template <typename TPrecisionType = float>
    void displayFuzzyRules(AI::FuzzyLogic::FuzzySystem<TPrecisionType>& in_system)
    {
        if (ImGui::BeginTable("##table1", in_system.getInputs()[0].getLinguisticVariable().getValues().size() + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            //Columns
            ImGui::TableSetupColumn((in_system.getInputs()[0].getName() + "/" + in_system.getInputs()[1].getName()).c_str());

            for (const LinguisticValue<TPrecisionType>& input : in_system.getInputs()[0].getLinguisticVariable().getValues())
            {
                float font_size = ImGui::GetFontSize() * input.getName().size() / 2;
                ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
                ImGui::TableSetupColumn(input.getName().c_str());
            }

            //Row
            ImGui::TableHeadersRow();

            for (int row = 0; row < in_system.getInputs()[1].getLinguisticVariable().getValues().size(); ++row)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);

                ImGui::Text(in_system.getInputs()[1].getLinguisticVariable().getValues()[row].getName().c_str());

                int idElem = 0;

                for (int column = 1; column < in_system.getInputs()[0].getLinguisticVariable().getValues().size() + 1; ++column)
                {
                    ImGui::TableSetColumnIndex(column);

                    idElem = row * in_system.getInputs()[0].getLinguisticVariable().getValues().size() + (column - 1);
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

    template <typename TPrecisionType = float>
    void displayFuzzySystemSolvePanel(AI::FuzzyLogic::FuzzySystem<TPrecisionType>& in_system)
    {
        if (ImGui::CollapsingHeader("Result"))
        {
            ImGui::Text("Settings");

            for (FuzzyValue<TPrecisionType>& input : in_system.getInputs())
            {
                TPrecisionType val = input.getValue();

                //TODO: create template slider for ImGui
                if (ImGui::SliderFloat((input.getName() + "##SliderInput").c_str(), &val, input.getLinguisticVariable().getMin(), input.getLinguisticVariable().getMax()))
                {
                    input.setValue(val);
                }
            }

            ImGui::Text("Fuzzification : ");
            TPrecisionType rst = 0.f;

            //Fuzzycation
            FuzzySet::FuzzySet<TPrecisionType> fuzzification(in_system.getOutput().getMin(), in_system.getOutput().getMax());
            fuzzification.add(in_system.getOutput().getMin(), static_cast<TPrecisionType>(0));
            fuzzification.add(in_system.getOutput().getMax(), static_cast<TPrecisionType>(0));
            for (const FuzzyRule<TPrecisionType>& rule : in_system.getRules())
            {
                FuzzySet::FuzzySet<TPrecisionType> resultingSet;
                if (rule.tryApply(resultingSet, in_system.getInputs()))
                {
                    fuzzification = fuzzification | resultingSet;
                }
            }

            //Defuzzification
            rst = fuzzification.centroid();\

                displayFuzzysificationAndDeffuzification(fuzzification, rst, in_system.getOutput().getName(), in_system.getOutput().getUnit());
            ImGui::Text("Deffuzzification : ");  ImGui::SameLine();  ImGui::Text(std::to_string(rst).c_str());
        }
    }

    template <typename TPrecisionType = float>
    void displayFuzzySystemSetupPanel(AI::FuzzyLogic::FuzzySystem<TPrecisionType>& in_system)
    {
        if (ImGui::Button("Add input"))
        {
            in_system.addInputVariable(LinguisticVariable<TPrecisionType>());
        }

        for (auto&& input : in_system.getInputs())
        {
            displayEditableLinguisticVariable(input.getLinguisticVariable());
        }

        if (ImGui::Button("Set outuput"))
        {
            in_system.setOutput(LinguisticVariable<TPrecisionType>());
        }
        
        displayEditableLinguisticVariable(in_system.getOutput());

        displayFuzzySystemSolvePanel(in_system);
    }
} /*namespace AI::FuzzyLogic::GUI*/