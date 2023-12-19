#include "ImGui_Widget_Bezier.h"
#include "ofApp.h"

float ImGui::BezierValue(float dt01, float P[8])
{
	enum { STEPS = 256 };
	ImVec2 Q[4] = { { P[0], P[1] }, { P[2], P[3] }, { P[4], P[5] }, { P[6], P[7] } };
	ImVec2 results[STEPS + 1];
	bezier_table<STEPS>(Q, results);
	return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
}

int ImGui::Bezier(const char* label, float P[8])
{
	// TODO:
	// Pause rendering when moving handles?
	// Update on changing presets

	// visuals
	enum { SMOOTHNESS = 100 }; // curve smoothness: the higher number of segments, the smoother curve
	enum { CURVE_WIDTH = 2 }; // main curved line width
	enum { LINE_WIDTH = 1 }; // handlers: small lines width
	enum { GRAB_RADIUS = 4 }; // handlers: circle radius
	enum { GRAB_BORDER = 0 }; // handlers: circle border width
	enum { AREA_CONSTRAINED = false }; // should grabbers be constrained to grid area?
	enum { AREA_WIDTH = 100 }; // area width in pixels. 0 for adaptive size (will use max avail width)

	// curve presets
	static struct { const char* name; float points[8]; } presets[] = {
		{ "Linear", 0.000f, 0.000f, 1.000f, 1.000f, 1, 1 },

		{ "In Sine", 0, 0, 0.470f, 0.000f, 0.745f, 0.715f, 1, 1 },
		{ "In Quad", 0, 0, 0.550f, 0.085f, 0.680f, 0.530f, 1, 1 },
		{ "In Cubic", 0, 0, 0.550f, 0.055f, 0.675f, 0.190f, 1, 1 },
		{ "In Quart", 0, 0, 0.895f, 0.030f, 0.685f, 0.220f, 1, 1 },
		{ "In Quint", 0, 0, 0.755f, 0.050f, 0.855f, 0.060f, 1, 1 },
		{ "In Expo", 0, 0, 0.950f, 0.050f, 0.795f, 0.035f, 1, 1 },
		{ "In Circ", 0, 0, 0.600f, 0.040f, 0.980f, 0.335f, 1, 1 },
		{ "In Back", 0, 0, 0.600f, -0.28f, 0.735f, 0.045f, 1, 1 },
		{ "Out Sine", 0, 0, 0.390f, 0.575f, 0.565f, 1.000f, 1, 1 },
		{ "Out Quad", 0, 0, 0.250f, 0.460f, 0.450f, 0.940f, 1, 1 },
		{ "Out Cubic", 0, 0, 0.215f, 0.610f, 0.355f, 1.000f, 1, 1 },
		{ "Out Quart", 0, 0, 0.165f, 0.840f, 0.440f, 1.000f, 1, 1 },
		{ "Out Quint", 0, 0, 0.230f, 1.000f, 0.320f, 1.000f, 1, 1 },
		{ "Out Expo", 0, 0, 0.190f, 1.000f, 0.220f, 1.000f, 1, 1 },
		{ "Out Circ", 0, 0, 0.075f, 0.820f, 0.165f, 1.000f, 1, 1 },
		{ "Out Back", 0, 0, 0.175f, 0.885f, 0.320f, 1.275f, 1, 1 },

		{ "InOut Sine", 0, 0, 0.445f, 0.050f, 0.550f, 0.950f, 1, 1 },
		{ "InOut Quad", 0, 0, 0.455f, 0.030f, 0.515f, 0.955f, 1, 1 },
		{ "InOut Cubic", 0, 0, 0.645f, 0.045f, 0.355f, 1.000f, 1, 1 },
		{ "InOut Quart", 0, 0, 0.770f, 0.000f, 0.175f, 1.000f, 1, 1 },
		{ "InOut Quint", 0, 0, 0.860f, 0.000f, 0.070f, 1.000f, 1, 1 },
		{ "InOut Expo", 0, 0, 1.000f, 0.000f, 0.000f, 1.000f, 1, 1 },
		{ "InOut Circ", 0, 0, 0.785f, 0.135f, 0.150f, 0.860f, 1, 1 },
		{ "InOut Back", 0, 0, 0.680f, -0.55f, 0.265f, 1.550f, 1, 1 }

	};


	// preset selector
	/*
	bool reload = 0;
	ImGui::PushID(label);
	if (ImGui::ArrowButton("##lt", ImGuiDir_Left)) {
		if (--P[8] >= 0) reload = 1; else ++P[8];
	}
	ImGui::SameLine();

	if (ImGui::Button("Presets")) {
		ImGui::OpenPopup("!Presets");
	}
	if (ImGui::BeginPopup("!Presets")) {
		for (int i = 0; i < IM_ARRAYSIZE(presets); ++i) {
			if (i == 1 || i == 9 || i == 17) ImGui::Separator();
			if (ImGui::MenuItem(presets[i].name, NULL, P[8] == i)) {
				P[8] = i;
				reload = 1;
			}
		}
		ImGui::EndPopup();
	}
	ImGui::SameLine();

	if (ImGui::ArrowButton("##rt", ImGuiDir_Right)) { 
		if (++P[8] < IM_ARRAYSIZE(presets)) reload = 1; else --P[8];
	}
	ImGui::SameLine();
	ImGui::PopID();

	if (reload) {
		memcpy(P, presets[(int)P[8]].points, sizeof(float) * 8);
	}
	*/

	// bezier widget

	const ImGuiStyle& Style = GetStyle();
	const ImGuiIO& IO = GetIO();
	ImDrawList* DrawList = GetWindowDrawList();
	ImGuiWindow* Window = GetCurrentWindow();
	if (Window->SkipItems)
		return false;

	// header and spacing
	int changed = 0;
	// int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
	int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?

	// prepare canvas
	const float avail = GetContentRegionAvail().x;
	const float dim = AREA_WIDTH > 0 ? AREA_WIDTH : avail;
	ImVec2 Canvas(dim, dim);

	ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return changed;

	const ImGuiID id = Window->GetID(label);
	hovered |= 0 != ItemHoverable(ImRect(bb.Min, bb.Min + ImVec2(avail, dim)), id);

	RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

	// background grid
	for (int i = 0; i <= Canvas.x; i += (Canvas.x / 6)) {
		DrawList->AddLine(
			ImVec2(bb.Min.x + i, bb.Min.y),
			ImVec2(bb.Min.x + i, bb.Max.y),
			GetColorU32(ImGuiCol_TextDisabled));
	}
	for (int i = 0; i <= Canvas.y; i += (Canvas.y / 6)) {
		DrawList->AddLine(
			ImVec2(bb.Min.x, bb.Min.y + i),
			ImVec2(bb.Max.x, bb.Min.y + i),
			GetColorU32(ImGuiCol_TextDisabled));
	}

	// eval curve
	ImVec2 Q[4] = { { P[0], P[1] }, { P[2], P[3] }, { P[4], P[5] }, { P[6], P[7] } };
	ImVec2 results[SMOOTHNESS + 1];
	bezier_table<SMOOTHNESS>(Q, results);

	{
		// handle grabbers
		ImVec2 mouse = GetIO().MousePos, pos[4];
		float distance[4];

		for (int i = 0; i < 4; ++i) {
			pos[i] = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
			distance[i] = (pos[i].x - mouse.x) * (pos[i].x - mouse.x) + (pos[i].y - mouse.y) * (pos[i].y - mouse.y);
		}

		// Assume the first element is the minimum
		int selected = 0;
		int minValue = distance[0];
		for (int i = 1; i < 4; ++i) {
			if (distance[i] < minValue) {
				minValue = distance[i];  // Update minValue
				selected = i;
			}
		}

		if (distance[selected] < (8 * GRAB_RADIUS * 8 * GRAB_RADIUS))
		{
			SetTooltip("(%4.3f, %4.3f)", P[selected * 2 + 0], P[selected * 2 + 1]);

			if (/*hovered &&*/ (IsMouseClicked(0) || IsMouseDragging(0))) {
				float& px = (P[selected * 2 + 0] += GetIO().MouseDelta.x / Canvas.x);
				float& py = (P[selected * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y);

				if (AREA_CONSTRAINED || selected == 0 || selected == 3) {
					px = (px < 0 ? 0 : (px > 1 ? 1 : px));
					py = (py < 0 ? 0 : (py > 1 ? 1 : py));
				}

				changed = true;
			}
		}
	}

	// if (hovered || changed) DrawList->PushClipRectFullScreen();

	// draw curve
	{
		ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
		for (int i = 0; i < SMOOTHNESS; ++i) {
			ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
			ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
			ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
			ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
			DrawList->AddLine(r, s, color, CURVE_WIDTH);
		}
	}

	// draw lines and grabbers
	float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
	ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
	ImVec4 grey(0.75f, 0.75f, 0.75f, luma);
	ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
	ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
	ImVec2 p3 = ImVec2(P[4], 1 - P[5]) * (bb.Max - bb.Min) + bb.Min;
	ImVec2 p4 = ImVec2(P[6], 1 - P[7]) * (bb.Max - bb.Min) + bb.Min;
	DrawList->AddLine(p1, p2, ImColor(white), LINE_WIDTH);
	DrawList->AddLine(p4, p3, ImColor(white), LINE_WIDTH);
	DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
	DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(grey));
	DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
	DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(grey));
	DrawList->AddCircleFilled(p3, GRAB_RADIUS, ImColor(white));
	DrawList->AddCircleFilled(p3, GRAB_RADIUS - GRAB_BORDER, ImColor(grey));
	DrawList->AddCircleFilled(p4, GRAB_RADIUS, ImColor(white));
	DrawList->AddCircleFilled(p4, GRAB_RADIUS - GRAB_BORDER, ImColor(grey));

	// if (hovered || changed) DrawList->PopClipRect();

	return changed;
}

void ImGui::ShowBezierDemo()
{
	{ static float v[8] = { 0, 0, 0.950f, 0.050f, 0.795f, 0.035f, 1, 1 }; Bezier("easeInExpo", v); }
}
