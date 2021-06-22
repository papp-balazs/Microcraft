#include "Game.h"
#include "ThreadSettings.h"
#include "GDIPlusManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include <optional>
#include <memory>

GDIPlusManager gdipm;

Game::Game(size_t width, size_t height) : wnd(width, height)
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, (float)height / (float)width, 0.5f, 20.0f));

	if (!showCursor)
	{
		wnd.DisableCursor();
		wnd.mouse.EnableRaw();
	}
}

Game::~Game()
{}

void Game::DoFrame()
{
#ifdef THREADED
	while (!exit)
	{
#endif
		const auto dt = timer.Mark();
		wnd.Gfx().BeginFrame(0.5f * skyIntensity, 0.91f * skyIntensity, 1.0f * skyIntensity);
		wnd.Gfx().SetCamera(cam.GetMatrix());

		while (auto e = wnd.kbd.ReadKey())
		{
			if (e->GetCode() == VK_SHIFT)
			{
				if (e->IsPress())
				{
					cam.SetTravelSpeed(40.0f);
				}
				else
				{
					cam.SetTravelSpeed(20.0f);
				}

				continue;
			}

			if (!e->IsPress())
			{
				continue;
			}

			switch (e->GetCode())
			{
			case VK_ESCAPE:
				if (!showCursor)
				{
					wnd.EnableCursor();
					showCursor = true;
					wnd.mouse.DisableRaw();
				}

				break;
			}
		}

		if (!showCursor)
		{
			if (wnd.kbd.KeyIsPressed('W'))
			{
				cam.Translate({ 0.0f, 0.0f, dt });
			}

			if (wnd.kbd.KeyIsPressed('A'))
			{
				cam.Translate({ -dt, 0.0f, 0.0f });
			}

			if (wnd.kbd.KeyIsPressed('S'))
			{
				cam.Translate({ 0.0f, 0.0f, -dt });
			}

			if (wnd.kbd.KeyIsPressed('D'))
			{
				cam.Translate({ dt, 0.0f, 0.0f });
			}

			if (wnd.kbd.KeyIsPressed(VK_SPACE))
			{
				cam.Translate({ 0.0f, dt, 0.0f });
			}

			if (wnd.kbd.KeyIsPressed(VK_CONTROL))
			{
				cam.Translate({ 0.0f, -dt, 0.0f });
			}
		}

		while (auto ms = wnd.mouse.Read())
		{
			if (showCursor && ms->GetType() == Mouse::Event::Type::LeftPress && wnd.mouse.IsInWindow())
			{
				wnd.DisableCursor();
				showCursor = false;
				wnd.mouse.EnableRaw();
			}
		}

		while (const auto delta = wnd.mouse.ReadRawDelta())
		{
			if (!showCursor)
			{
				cam.Rotate(delta->x, delta->y);
			}
		}

		if (ImGui::Begin("Performace"))
		{
			ImGui::Text("Framerate: %.3f fps", ImGui::GetIO().Framerate);
			ImGui::Text("Frametime: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::End();
		}

		wnd.Gfx().EndFrame();
#ifdef THREADED
	}
#endif
}

int Game::Start()
{
#ifdef THREADED
	std::thread tr(&Game::DoFrame, this);
#endif
	while (1)
	{
		if (const auto result = Window::ProcessMessages())
		{
#ifdef THREADED
			exit = true;
			tr.join();
#endif
			return result.value();
		}
#ifndef THREADED
		DoFrame();
#endif
	}
}