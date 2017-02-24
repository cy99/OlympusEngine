#include "Localization.hpp"
#include "ParserIni.hpp"

namespace oe
{

template<> Localization* Singleton<Localization>::mSingleton = nullptr;

Localization::Localization()
	: mLanguages()
	, mTokens()
	, mCurrentLanguage(Language::English)
{
}

Localization& Localization::getSingleton()
{
	ASSERT(mSingleton != nullptr);
	return *mSingleton;
}

Localization* Localization::getSingletonPtr()
{
	return mSingleton;
}

void Localization::addLanguage(Language language, LanguageStrategy loadingStrategy)
{
	bool found = false;
	for (U32 i = 0; i < mLanguages.size(); i++)
	{
		if (mLanguages[i].id == language)
		{
			found = true;
		}
	}
	if (!found && loadingStrategy)
	{
		mLanguages.emplace_back(language, loadingStrategy);
	}
}

bool Localization::useLanguage(Language language)
{
	for (U32 i = 0; i < mLanguages.size(); i++)
	{
		if (mLanguages[i].id == language)
		{
			mTokens.clear();
			mCurrentLanguage = language;
			return mLanguages[i].strategy(mTokens);
		}
	}
	return false;
}

Localization::Language Localization::getCurrentLanguage() const
{
	return mCurrentLanguage;
}

const std::string& Localization::getToken(U32 token)
{
	return StringHash::get(mTokens[token]);
}

bool Localization::loadLanguageFromFile(LanguageTable& table, const std::string& filename)
{
	ParserIni iniParser;
	if (iniParser.loadFromFile(filename))
	{
		U32 size = iniParser.getSize();
		for (U32 i = 0; i < size; i++)
		{
			// TODO : Load from file
		}
		return true;
	}
	return false;
}

Localization::LanguageData::LanguageData(Language index, LanguageStrategy loadingStrategy)
	: id(index)
	, strategy(loadingStrategy)
{
}

} // namespace oe