using System;
using System.Windows.Forms;

namespace Mouz.Windows;

/// <summary>
/// Initial Windows platform host for Mouz applications.
/// Mouz source remains the application language; this C# assembly is backend infrastructure.
/// </summary>
public sealed class MouzWindowHost
{
    private readonly Form _window = new();

    public MouzWindowHost(string title, int width = 800, int height = 600)
    {
        _window.Text = title;
        _window.Width = Math.Max(320, width);
        _window.Height = Math.Max(240, height);
        _window.StartPosition = FormStartPosition.CenterScreen;
    }

    public void AddText(string value)
    {
        _window.Controls.Add(new Label
        {
            AutoSize = true,
            Text = value,
            Left = 16,
            Top = 16
        });
    }

    public void AddButton(string label, Action onClick)
    {
        ArgumentNullException.ThrowIfNull(onClick);
        var button = new Button
        {
            AutoSize = true,
            Text = label,
            Left = 16,
            Top = _window.Controls.Count * 42 + 16
        };
        button.Click += (_, _) => onClick();
        _window.Controls.Add(button);
    }

    public void Run()
    {
        Application.Run(_window);
    }
}
