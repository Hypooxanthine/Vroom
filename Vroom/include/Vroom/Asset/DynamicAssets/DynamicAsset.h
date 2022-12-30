#pragma once

namespace Vroom
{
	class StaticAsset;

	class DynamicAsset
	{
	public:
		using StaticType = StaticAsset;

	public:
		DynamicAsset() = default;
		DynamicAsset(const DynamicAsset&) = delete;

		virtual ~DynamicAsset();

		void setParent(const StaticAsset* parent) { m_Parent = parent; }

	private:
		const StaticAsset* m_Parent = nullptr;
	};
}