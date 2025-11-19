#include <windows.h>

struct MeasureData
{
    double Value = 0.0;
};

// Export everything with C linkage so Rainmeter can find the symbols
extern "C"
{
    __declspec(dllexport) void Initialize(void** data, void* rm)
    {
        // Called once per a_Measure instance
        auto* a_Measure = new MeasureData{};
        *data = a_Measure;
    }

    __declspec(dllexport) void Finalize(void* data)
    {
        // Called when the a_Measure is destroyed
        auto* a_Measure = static_cast<MeasureData*>(data);
        delete a_Measure;
    }

    __declspec(dllexport) void Reload(void* data, void* rm, double* maxValue)
    {
        // Called whenever the skin is refreshed or options change
        auto* a_Measure = static_cast<MeasureData*>(data);
        a_Measure->Value = 0.0;

        // Tell Rainmeter what the maximum Value will roughly be (for scaling bars)
        if (maxValue)
        {
            *maxValue = 1.0;
        }
    }

    __declspec(dllexport) double Update(void* data)
    {
        // Called every "Update=" ms from the skin
        auto* a_Measure = static_cast<MeasureData*>(data);

        // For now, just loop Value from 0..1 so we can see it working
        a_Measure->Value += 0.01;
        if (a_Measure->Value > 1.0)
        {
            a_Measure->Value = 0.0;
        }

        return a_Measure->Value;
    }

    __declspec(dllexport) LPCWSTR GetString(void* /*data*/)
    {
        // This plugin doesn't return strings (only numbers)
        return nullptr;
    }

} // extern "C"
