#include <imgui.h>

#include <Profiler.hpp>

#include <Application.hpp>
#include <ProfilerLayer.hpp>

#include <stack>

void DrawAsTable();
void DrawAsText();

void DrawMarkerTreeView();
bool BeginTree(Scaffold::Marker* marker);
void DrawMarker(Scaffold::Marker* marker);
void EndTree();

void ProfilerLayer::OnRenderUI(float deltaTime)
{
	static int selection = 0;

	bool show = ImGui::Begin("Profiler");
	if (!show) { ImGui::End(); return; }

	static int lastFrameCount = 0;
	static float lastFrameTime = 0;

	static int accumulatedFrames = 0;
	static float accumulatedTime = 0;

	if (accumulatedTime > 1.0f)
	{
		lastFrameCount = accumulatedFrames;
		lastFrameTime = accumulatedTime / accumulatedFrames;

		accumulatedTime = 0;
		accumulatedFrames = 0;
	}

	else
	{
		accumulatedFrames++;
		accumulatedTime += deltaTime;
	}

	ImGui::Text("Frame timing: %.2f FPS (%.4fms)", 1.0f / deltaTime, deltaTime * 1000.0f);
	ImGui::Text("1 sec window: %i FPS (%.4fms)", lastFrameCount, lastFrameTime * 1000.0f);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::RadioButton("As Table", &selection, 0); ImGui::SameLine();
	ImGui::RadioButton("As Text", &selection, 1);

	if (selection == 0) DrawAsTable();
	else if (selection == 1) DrawAsText();

	ImGui::End();
}

// Helpers
void DrawAsText()
{
	std::string report = Scaffold::Application::GetProfiler().GenerateReport();
	ImGui::TextUnformatted(report.c_str());
}

void DrawAsTable()
{
	static ImGuiTableFlags flags =
		ImGuiTableFlags_BordersV |
		ImGuiTableFlags_BordersOuterH |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_RowBg |
		ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("Frame Timing", 2, flags))
	{
		ImGui::TableSetupColumn("Marker", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Duration");
		ImGui::TableHeadersRow();

		DrawMarkerTreeView();
	}
	ImGui::EndTable();
}

void DrawMarkerTreeView()
{
	Scaffold::Marker& rootMarker = Scaffold::Application::GetProfiler().GetRootMarker();

	if (rootMarker.subMarkers.size() == 0) return;

	std::stack<Scaffold::Marker*> markerStack;
	std::stack<bool> treeStack;

	markerStack.push(&rootMarker);
	treeStack.push(false);

	while (!markerStack.empty())
	{
		Scaffold::Marker* marker = markerStack.top();
		markerStack.pop();

		bool isTreeRoot = treeStack.top();
		treeStack.pop();

		if (!isTreeRoot)
		{
			int subMarkerCount = marker->subMarkers.size();

			if (subMarkerCount > 0)
			{
				if (!BeginTree(marker))
				{
					continue;
				};

				markerStack.push(marker);
				treeStack.push(true);

				for (int i = subMarkerCount - 1; i > -1; i--)
				{
					markerStack.push(&marker->subMarkers[i]);
					treeStack.push(false);
				}
			}

			else
			{
				DrawMarker(marker);
			}
		}

		else
		{
			EndTree();
		}
	}
}

void DrawMarkerDuration(Scaffold::Marker* marker)
{
	if (marker->measurementCount > 1)
	{
		ImGui::TextDisabled("%.5fms (count: %i)", marker->durationAsMilliseconds, (int)marker->measurementCount);
	}

	else
	{
		ImGui::TextDisabled("%.4fms", marker->durationAsMilliseconds);
	}
}

bool BeginTree(Scaffold::Marker* marker)
{
	static ImGuiTreeNodeFlags flags =
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_SpanFullWidth;

	ImGui::TableNextRow();

	ImGui::TableNextColumn();
	bool isOpen = ImGui::TreeNodeEx(marker->name.c_str(), flags);

	ImGui::TableNextColumn();
	DrawMarkerDuration(marker);
	return isOpen;
}

void EndTree()
{
	ImGui::TreePop();
}

void DrawMarker(Scaffold::Marker* marker)
{
	ImGui::Indent(20);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::TextDisabled("%s", marker->name.c_str());
	ImGui::TableNextColumn();

	DrawMarkerDuration(marker);

	ImGui::Indent(-20);
}
