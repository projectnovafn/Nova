#include "VivoxVoiceChat.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>

#define VIVOX_SECRET "_"
#define VIVOX_ISSUER "_"
#define VIVOX_DOMAIN "_"

using namespace std;

typedef unsigned char uchar;
static const string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_+/";

static string base64_encode(const string& in)
{
    string out;

    int val = 0, valb = -6;
    for (const char c : in)
    {
        val = (val << 8) + static_cast<unsigned char>(c);
        valb += 8;
        while (valb >= 0)
        {
            out.push_back(b[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6) out.push_back(b[((val << 8) >> (valb + 8)) & 0x3F]);

    return out;
}

std::string hmac(const std::string& key, const std::string& data)
{
    unsigned char* hash = HMAC(EVP_sha256(), key.c_str(), key.length(),
        reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), nullptr, nullptr);

    stringstream ss;
    ss << setfill('0');
    for (unsigned int i = 0; i < 32; i++) {
        ss << hash[i];
    }

    return ss.str();
}

static string vx_generate_token(const string& key, const string& issuer, int exp, const string& vxa, int vxi, const string& f, const string& t)
{
    string header = base64_encode("{}");

    stringstream ss;
    ss << "{ \"iss\": \"" << issuer << "\",";
    ss << "\"exp\": " << exp << ",";
    ss << "\"vxa\": \"" << vxa << "\",";
    ss << "\"vxi\": " << vxi << ",";
    ss << "\"f\": \"" << f << "\",";
    ss << "\"t\": \"" << t << "\" }";
    string payload = base64_encode(ss.str());

    string to_sign = header + "." + payload;
    string signed_payload = base64_encode(hmac(key, to_sign));

    return to_sign + "." + signed_payload;
}

FString FVivoxVoiceChat::GetJoinToken(FUniqueNetIdRepl& UniqueId, const FString& ChannelName)
{
    auto exp = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count() + (60 * 60);

    auto from = std::format("sip:.{}.{}.@{}", VIVOX_ISSUER, UniqueId.ToString().ToString(), VIVOX_DOMAIN);
    auto to = std::format("sip:confctl-g-{}.s-{}@{}", VIVOX_ISSUER, ChannelName.ToString(), VIVOX_DOMAIN);

    return vx_generate_token(VIVOX_SECRET, VIVOX_ISSUER, exp, "join", 
        FMath::Rand() % 10000, from, to);
}

FVivoxVoiceChat* FVivoxVoiceChat::Get()
{
	static auto VivoxVoiceChat = new FVivoxVoiceChat();
	return VivoxVoiceChat;
}