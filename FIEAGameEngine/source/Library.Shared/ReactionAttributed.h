#pragma once
#include "Reaction.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:
		explicit ReactionAttributed(const std::string& name, const std::string& subType);
		ReactionAttributed(const ReactionAttributed& rhs);
		ReactionAttributed(ReactionAttributed&& rhs) noexcept;
		ReactionAttributed& operator=(const ReactionAttributed&) = default;
		ReactionAttributed& operator=(ReactionAttributed&&) noexcept = default;
		~ReactionAttributed();

		const std::string& SubType() const;
		void SetSubType(const std::string& subType);

		gsl::owner<ReactionAttributed*> Clone() const override;
		virtual void Notify(const EventPublisher& event) override;

		static const Vector<Signature> Signatures();

	private:
		static void CopyParameters(const Vector<Scope>& parameters, Attributed& destination);
		std::string _subType;
	};
}