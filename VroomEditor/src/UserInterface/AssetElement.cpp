#include "VroomEditor/UserInterface/AssetElement.h"

using namespace vrm;

AssetElement::AssetElement(const std::filesystem::path& elementPath)
    : ImGuiElement(), m_ElementPath(elementPath)
{

}

AssetElement::~AssetElement()
{
    
}
