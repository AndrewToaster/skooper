#include "SkpGist.h"
#include "SkpUtil.h"
#include "SkpCommon.h"

#define SKP_BIN_NEW_PASTE "https://pastebin.com/api/api_post.php"
#define SKP_GIST_IPFILE_ID "b5fe4133022adb9b1720220d881994d6"

#define SKP_GIST_PATH "https://api.github.com/gists/" SKP_GIST_IPFILE_ID
#define SKP_GIST_RAW "https://gist.githubusercontent.com/AndrewToaster/b5fe4133022adb9b1720220d881994d6/raw/ipfile.txt"

static HTTPClient http;

bool skp_gist_updateIPFile(const String& ip)
{
    if (!http.begin(SKP_GIST_PATH))
    {
        skp_log_error("Failed to parse url");
        return false;
    }

    http.addHeader("Content-Type", MIME_JSON);
    http.addHeader("Accept", "application/vnd.github+json");
    http.addHeader("X-GitHub-Api-Version", "2022-11-28");
    http.addHeader("Authorization", "Bearer " GIST_KEY);

    auto resp = http.PATCH(
        "{\"description\": \"Pleaswe wowk? :333\", \"files\": { \"ipfile.txt\" : { \"content\": \"" + ip + "\"}}}"
    );

    if (resp < 0)
    {
        skp_log_error(String("Internal error: ") + http.errorToString(resp));
        http.end();
        return false;
    }
    else if (resp != 200)
    {
        skp_log_error(String("Incorrect response code: ") + resp + "\n-----\n" + http.getString() + "\n------");
        http.end();
        return false;
    }

    skp_log_debug(http.getString());

    http.end();
    return true;
}

String skp_gist_readIPFile()
{
    if (!http.begin(SKP_GIST_RAW))
    {
        skp_log_error("Failed to parse url");
        return emptyString;
    }

    auto resp = http.GET();
    skp_log_debug(resp);
    if (resp < 0)
    {
        skp_log_error(String("Internal error: ") + http.errorToString(resp));
        http.end();
        return emptyString;
    }
    else if (resp != 200)
    {
        skp_log_error(String("Incorrect response code: ") + resp + "\n-----\n" + http.getString() + "\n------");
        http.end();
        return emptyString;
    }
    
    auto content = http.getString();
    http.end();

    skp_log_debug(content);

    return content;
}