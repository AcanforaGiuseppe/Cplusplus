#include <iostream>
#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>

struct JSONNode;
using jsonNumber = double;
using jsonString = std::string;
using jsonBool = bool;
using jsonArray = std::vector<std::shared_ptr<JSONNode>>;
using jsonObject = std::unordered_map<std::string, std::shared_ptr<JSONNode>>;
using jsonVariant = std::variant<jsonNumber, jsonString, jsonBool, jsonArray, jsonObject>;

struct JSONNode
{
	std::optional<jsonVariant> Value;
	std::shared_ptr<JSONNode> Parent;

	JSONNode()
	{
		Value = std::nullopt;
		Parent = nullptr;
	}

	JSONNode(std::shared_ptr<JSONNode> InParent) : Parent(InParent)
	{ }

	JSONNode(std::shared_ptr<JSONNode> InParent, std::string StringValue) : Parent(InParent), Value(StringValue)
	{ }

	JSONNode(std::shared_ptr<JSONNode> InParent, double InDouble) : Parent(InParent), Value(InDouble)
	{ }

	template <typename T>
	bool Is() const
	{
		return Value.has_value() ? std::holds_alternative<T>(Value.value()) : false;
	}

	template <typename T>
	T& As()
	{
		return std::get<T>(Value.value());
	}
};

std::ostream& operator<<(std::ostream& os, JSONNode& node)
{
	if (node.Is<jsonString>())
	{
		os << node.As<jsonString>();
	}
	else if (node.Is<jsonNumber>())
	{
		os << node.As<jsonNumber>();
	}
	else if (node.Is<jsonArray>())
	{
		os << "[ ";

		for (std::shared_ptr<JSONNode>& ChildNode : node.As<jsonArray>())
			os << *ChildNode << ", ";

		os << " ]";
	}
	else if (node.Is<jsonObject>())
	{
		os << "{ ";

		for (auto& pair : node.As<jsonObject>())
			os << pair.first << " : " << *pair.second << ", ";

		os << " }";
	}
	else
	{
		os << "AAAA";
	}

	return os;
}
enum class JSONTokenType
{
	CurlyBracketOpen,
	CurlyBracketClosed,
	Colon,
	String,
	Number,
	SquareBracketOpen,
	SquareBracketClosed,
	Comma,
	Boolean,
	Null,
};

struct JSONToken
{
	JSONTokenType Type;
	std::string Value;

	JSONToken(JSONTokenType InType) : Type(InType)
	{ }
};

bool JSONTokenizer(std::string json, std::vector<JSONToken>& tokens)
{
	size_t offset = 0;

	while (offset < json.size())
	{
		char c = json[offset++];

		if (std::isspace(c))
			continue;

		if (c == '"')
		{
			JSONToken token(JSONTokenType::String);
			bool isValid = false;

			while (offset < json.size())
			{
				c = json[offset++];

				if (c == '"')
				{
					isValid = true;
					break;
				}

				token.Value += c;
			}

			if (!isValid)
				return false;

			tokens.push_back(token);
		}
		else if (c == '{')
		{
			tokens.push_back(JSONToken(JSONTokenType::CurlyBracketOpen));
		}
		else if (c == '}')
		{
			tokens.push_back(JSONToken(JSONTokenType::CurlyBracketClosed));
		}
		else if (c == '[')
		{
			tokens.push_back(JSONToken(JSONTokenType::SquareBracketOpen));
		}
		else if (c == ']')
		{
			tokens.push_back(JSONToken(JSONTokenType::SquareBracketClosed));
		}
		else if (c == ':')
		{
			tokens.push_back(JSONToken(JSONTokenType::Colon));
		}
		else if (c == ',')
		{
			tokens.push_back(JSONToken(JSONTokenType::Comma));
		}
		else if (c == '+' || c == '-' || (c >= '0' && c <= '9'))
		{
			JSONToken token(JSONTokenType::Number);
			token.Value += c;
			bool isValid = false;
			bool isDot = false;

			while (offset < json.size())
			{
				c = json[offset++];
				if (c == '.')
				{
					if (isDot)
						return false;

					isDot = true;
					token.Value += c;
				}
				else if (c >= '0' && c <= '9')
				{
					token.Value += c;
				}
				else
				{
					isValid = true;
					break;
				}
			}

			if (!isValid)
				return false;

			tokens.push_back(token);
			offset--;
		}
		else
		{
			return false;
		}
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const JSONTokenType& type)
{
	switch (type)
	{
	case JSONTokenType::CurlyBracketOpen:
		os << "CurlyBracketOpen";
		break;

	case JSONTokenType::CurlyBracketClosed:
		os << "CurlyBracketClosed";
		break;

	case JSONTokenType::Colon:
		os << "Colon";
		break;

	case JSONTokenType::String:
		os << "String";
		break;

	case JSONTokenType::Number:
		os << "Number";
		break;

	case JSONTokenType::SquareBracketOpen:
		os << "SquareBracketOpen";
		break;

	case JSONTokenType::SquareBracketClosed:
		os << "SquareBracketClosed";
		break;

	case JSONTokenType::Comma:
		os << "Comma";
		break;

	case JSONTokenType::Boolean:
		os << "Boolean";
		break;

	case JSONTokenType::Null:
		os << "Null";
		break;

	default:
		break;
	}

	return os;
}

int main(int argc, char* argv)
{
	std::string json = "{ \"isJson\": 12} ";

	std::string complexInlineJSON = "{\"integer\": 123, \"double\": 12.3, \"string\": \"Hello World\", \"lista\": [\"item1\", \"item2\", 12, 78.8], \"object\": {\"key1\": \"value2\", \"key3\": [1,2,3,4,5,6,7,8, \"end\"] }}";

	jsonVariant variant = 15.0;
	std::cout << std::holds_alternative<jsonNumber>(variant) << std::endl;
	std::cout << "Value is " << std::get<jsonNumber>(variant) << std::endl;

	std::vector<JSONToken> tokens;
	std::shared_ptr<JSONNode> currentNode = nullptr;
	std::shared_ptr<JSONNode> rootNode = nullptr;

	bool Colon = false;
	std::string currentKey;

	if (JSONTokenizer(complexInlineJSON, tokens))
	{
		for (const JSONToken& token : tokens)
		{
			if (token.Type == JSONTokenType::CurlyBracketOpen)
			{
				currentNode = std::make_shared<JSONNode>(currentNode);
				currentNode->Value = jsonObject();

				if (rootNode == nullptr)
					rootNode = currentNode;

				if (currentNode->Parent && currentNode->Parent->Is<jsonObject>())
				{
					currentNode->Parent->As<jsonObject>()[currentKey] = currentNode;
					Colon = false;
				}
			}
			else if (token.Type == JSONTokenType::CurlyBracketClosed)
			{
				currentNode = currentNode->Parent;
			}
			else if (token.Type == JSONTokenType::SquareBracketOpen)
			{
				currentNode = std::make_shared<JSONNode>(currentNode);
				currentNode->Value = jsonArray();

				if (rootNode == nullptr)
					rootNode = currentNode;

				if (currentNode->Parent && currentNode->Parent->Is<jsonObject>())
				{
					currentNode->Parent->As<jsonObject>()[currentKey] = currentNode;
					Colon = false;
				}
			}
			else if (token.Type == JSONTokenType::SquareBracketClosed)
			{
				currentNode = currentNode->Parent;
			}
			else if (token.Type == JSONTokenType::String)
			{
				if (currentNode->Is<jsonArray>())
				{
					std::shared_ptr<JSONNode> newNode = std::make_shared<JSONNode>(currentNode, token.Value);
					std::cout << currentNode->As<jsonArray>().size() << std::endl;
					currentNode->As<jsonArray>().push_back(newNode);
				}
				else if (currentNode->Is<jsonObject>())
				{
					if (Colon == false)
					{
						currentKey = token.Value;
					}
					else
					{
						Colon = false;
						currentNode->As<jsonObject>()[currentKey] = std::make_shared<JSONNode>(currentNode, token.Value);
					}
				}
			}
			else if (token.Type == JSONTokenType::Number)
			{
				if (currentNode->Is<jsonArray>())
				{
					std::shared_ptr<JSONNode> newNode = std::make_shared<JSONNode>(currentNode, std::stod(token.Value));
					std::cout << currentNode->As<jsonArray>().size() << std::endl;
					currentNode->As<jsonArray>().push_back(newNode);
				}
				else if (currentNode->Is<jsonObject>())
				{
					Colon = false;
					currentNode->As<jsonObject>()[currentKey] = std::make_shared<JSONNode>(currentNode, std::stod(token.Value));
				}
			}
			else if (token.Type == JSONTokenType::Colon)
			{
				Colon = true;
			}
			std::cout << "Token Type is " << token.Type << " and value is " << token.Value << std::endl;
		}
		if (rootNode != nullptr)
		{
			if (rootNode->Is<jsonArray>())
				std::cout << "RootNode is an array \n";

			if (rootNode->Is<jsonObject>())
				std::cout << "RootNode is a JsonObject \n";

			std::cout << *rootNode << "\n";
		}
	}

	std::getchar();
}