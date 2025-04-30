#include "Curl/CurlHttp.h"
#include "Core.h"

bool FCurlHttpRequest::ProcessRequest()
{
    std::wstring URL(*GetURL());
    size_t PathIndex = URL.find(L"ol.epicgames.com");

    if (PathIndex != std::wstring::npos)
    {
        auto Path = URL.substr(PathIndex + 16);
        auto NewURL = API_URL + Path;

        SetURL(NewURL.c_str());
    }

    return true;
}