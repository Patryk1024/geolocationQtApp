#include "gtest/gtest.h"
#include "../../geolocationQtApp/RegexValidation.cpp"

#include <vector>
#include <QString>

TEST(RegexValidationTest, CheckCityNames)
{
	std::vector<QString> cityNames{ "Los Angeles", "Washington D.C.", "Pomiechówek", "Żywiec - Zdrój", "Toronto", "St.Catharines", "San Fransisco", "Val - d\'Or", "Presqu\'ile", "Niagara on the Lake", "Niagara - on - the - Lake", "München", "Kópavogur", "Garðabær", "Sauðárkrókur", "Þorlákshöfn" };
	bool valid = true;
	for (const auto& name : cityNames)
	{
		if (!RegexValidation::isValidCityName(name))
		{
			qDebug() << "CheckCityNames: " + name;
			valid = false;
			break;
		}
	}
	EXPECT_TRUE(valid);
}


TEST(RegexValidationTest, CheckUrls)
{
	std::vector<QString> goodUrls{ "https://www.example.com", "google.com", "http://api.ipstack.com/134.201.250.155?access_key=Some123ApI456", "https://pl.888.com/" };
	std::vector<QString> badUrls{ "invalid_url", "134.201.250.155", "134.201.250", "ftp://ftp.example.com" };
	bool good = true; // for good
	bool bad = true; // for bad
	for (const auto& url : goodUrls)
	{
		if (!RegexValidation::isValidUrl(url))
		{
			qDebug() << "CheckUrl: " + url;
			good = false;
			break;
		}
	}
	for (const auto& url : badUrls)
	{
		if (RegexValidation::isValidUrl(url))
		{
			qDebug() << "CheckUrl: " + url;
			bad = false;
			break;
		}
	}
	EXPECT_TRUE(good && bad);
}

TEST(RegexValidationTest, CheckIPv4s)
{
	std::vector<QString> goodIPs{ "127.0.0.1", "255.255.255.255", "134.201.250.155" };
	std::vector<QString> badIPs{ "421.214.251.215", "123.121", "127", "127.0.0", "134.201.250"};
	bool good = true; // for good
	bool bad = true; // for bad
	for (const auto& ip : goodIPs)
	{
		if (!RegexValidation::isValidIPv4(ip))
		{
			qDebug() << "CheckIP: " + ip;
			good = false;
			break;
		}
	}
	for (const auto& ip : badIPs)
	{
		if (RegexValidation::isValidIPv4(ip))
		{
			qDebug() << "CheckIP: " + ip;
			bad = false;
			break;
		}
	}
	EXPECT_TRUE(good && bad);
}

TEST(RegexValidationTest, CheckIPv6s)
{
	std::vector<QString> goodIPs{
"::",
"::1",
"::ffff:192.0.2.128",
"0:0:0:0:0:0:0:1",
"2600:9000:250b:d000:7:49a5:5fd3:b641",	//amazon
"1050:0:0:0:5:600:300c:1",
"2001::1",
"2001:0:9D38:953C:10EF:EE22:FFDD:AABB",
"2001:0DA8:0200:0012:0000:00B8:0000:02AA",
"2001:0db8::1",
"2001:0db8::1:0:0:1",
"2001:0DB8::4152:EBAF:CE01:0001",
"2001:0db8:0:0:1:0:0:1",
"2001:0DB8:0000:CD30:0000:0000:0000:0000",
"2001:0DB8:1234:5678:ABCD:EF01:2345:6789",
"2001:0db8:85a3:0000:0000:8a2e:0370:7334",
"2001:0db8:85a3:08d3:1319:8a2e:0370:7344",
"2001:0DB8:aaaa:0007:0000:0000:0000:0001",
"2001:2::10",
"2001:44b8:4126:f600:91bd:970c:9073:12df",
"2001:4860:4860::8888",
"2001:500:2d::d",
"2001:558:fc03:11:5e63:3eff:fe67:edf9",
"2001:acad:abad:1::bc",
"2001:b50:ffd3:76:ce58:32ff:fe00:e7",
"2001:db8::0:1:0:0:1",
"2001:db8::1",
"2001:db8::1:0:0:1",
"2001:db8::212:7403:ace0:1",
"2001:DB8::4:5:6:7",
"2001:db8::5",
"2001:DB8::8:800:200C:417A",
"2001:db8::aaaa:0:0:1",
"2001:db8:0::1",
"2001:db8:0:0::1",
"2001:db8:0:0:0::1",
"2001:db8:0:0:1::1",
"2001:DB8:0:0:1::1",
"2001:db8:0:0:1:0:0:1",
"2001:DB8:0:0:8:800:200C:417A",
"2001:db8:0:0:aaaa::1",
"2001:db8:0000:0:1::1",
"2001:db8:3c4d:15::1",
"2001:db8:0:1:1:1:256:1",
"2001:DB8:85A3::8A2E:370:7334",
"2001:db8:aaaa:bbbb:cccc:dddd::1",
"2001:db8:aaaa:bbbb:cccc:dddd:0:1",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:0001",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:001",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:01",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:1",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:aaaa",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:AAAA",
"2001:db8:aaaa:bbbb:cccc:dddd:eeee:AaAa",
"2001:db8:d03:bd70:fede:5c4d:8969:12c4",
"2002::8364:7777",
"2002:4559:1FE2::4559:1FE2",
"2002:C000:203:200::",
"2002:cb0a:3cdd:1:1:1:1:1",
"2400:8902::f03c:92ff:feb5:f66d",
"2400:c980:0:e206:b07d:8cf9:2b05:fb06",
"2400:cb00:2048:1::6814:507",
"2404:6800:4009:805::2004",
"2607:f8b0:4005:80b::200e",
"2607:f8b0:400a:809::200e",
"2620:0:1cfe:face:b00c::3",
"2620:0:2d0:200::7",
"3fff:ffff:3:1:0:0:0:7",
"ABCD:EF01:2345:6789:ABCD:EF01:2345:6789",
"fc00::",
"fd3b:d101:e37f:9713::1",
"fd44:a77b:40ca:db17:37df:f4c4:f38a:fc81",
"FD92:7065:891e:8c71:d2e7:d3f3:f595:d7d8",
"fe80::",
"fe80::cd8:95bf:afbb:9622",
"FE80:0000:0000:0000:0202:B3FF:FE1E:8329",
"fe80:dead:beef:cafe:face:feed:f12d:bedd",
"fec0:0:0:1::1",
"FEDC:BA98:7654:3210:FEDC:BA98:7654:3210",
"FF01::101",
"FF01:0:0:0:0:0:0:1",
"FF01:0:0:0:0:0:0:101",
"FF02::1",
"FF02:0:0:0:0:0:0:1",
"FF02:0:0:0:0:0:0:a",
"FF05:15:25:df:20:4a:b4:24",
"FF08:0:0:0:0:0:0:fc", };

	std::vector<QString> badIPs{
"::-1",
"::/0/0",
"::%eth0",
"::10.0.0.1",
"::255.255.255.255",
"::42:192.168.0.1",
"::ffff:0.0.0.256",
"::ffff:127.0.0.1/96",
"::ffff:192.0.2.128/33",
"::ffff:192.0.2.256",
"::ffff:192.168.1.256",
"0:0:0:0:0:0:192.168.0.1",
"1:2:3:4:5:6:7:8:9",
"1080:0:0:0:0:0:0:192.88.99",
"2001::0223:dead:beef::1",
"2001::192.168.0.1",
"2001::dead::beef",
"2001::ff4:2:1:1:1:1:1",
"2001:0DB8:0:CD3",
"2001:0db8:1234:5678:90AB:CDEF:0012:3456:789a",
"2001:10:240:ab::192.19.24.1",
"2001:db8:::1:0",
"2001:db8::1 ::2",
"2001:db8::192.168.0.1",
"2001:db8:/60",
"2001:db8:0:0:0:0:0/64",
"2001:db8:0:0:0:0:f:1g",
"2001:db8:0:0:0g00:1428:57ab",
"2001:db8:0:1:::1",
"2001:db8:0:1::/129",
"2001:db8:0:1::1::1",
"2001:db8:0:1::192.0.2.128",
"2001:db8:0:1::a:b:c:d:e:f",
"2001:db8:0:1:/64",
"2001:db8:0:1:1:1:1::1",
"2001:db8:0:1:1:1:1:1:1",
"2001:db8:0:1:1:1:1:1#test",
"2001:db8:0:1g:0:0:0:1",
"2001:db8:aaaa:bbbb:cccc:dddd-eeee:ffff",
"2001:dg8:0:0:0:0:1428:57ab",
"2001:gdba:0000:0000:0000:0000:3257:9652",
"2001:ggg:0:0:0:0:1428:57ab",
"2001.x:0:0:0:0:0:0:1",
"20011:db8:0:1:1:1:1:1",
"2403:780:f:102:a:a:1:0:0",
"2403:780:f:102:g:a:1:0",
"260.02:00a:b:10:abc:def:123f:2552",
"fe80:::1",
"fe80::/130",
"fe80::7::8",
"2001:0DB8:0:CD3", };

	bool good = true; // for good
	bool bad = true; // for bad
	for (const auto& ip : goodIPs)
	{
		if (!RegexValidation::isValidIPv6(ip))
		{
			qDebug() << "CheckIP: " + ip;
			good = false;
			break;
		}
	}
	for (const auto& ip : badIPs)
	{
		if (RegexValidation::isValidIPv6(ip))
		{
			qDebug() << "CheckIP: " + ip;
			bad = false;
			break;
		}
	}
	EXPECT_TRUE(good && bad);
}
