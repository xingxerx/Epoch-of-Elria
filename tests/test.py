"""Run this model in Python

> pip install azure-ai-inference
"""
import os
from azure.ai.inference import ChatCompletionsClient
from azure.ai.inference.models import AssistantMessage, SystemMessage, UserMessage, ToolMessage
from azure.ai.inference.models import ImageContentItem, ImageUrl, TextContentItem
from azure.core.credentials import AzureKeyCredential

# To authenticate with the model you will need to generate a personal access token (PAT) in your GitHub settings.
# Create your PAT token by following instructions here: https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens
client = ChatCompletionsClient(
    endpoint = "https://models.inference.ai.azure.com",
    credential = AzureKeyCredential(os.environ["GITHUB_TOKEN"]),
    api_version = "2024-08-01-preview",
)

response = client.complete(
    messages = [
        SystemMessage(content = "World's best coder"),
        UserMessage(content = [
            TextContentItem(text = "Update or game engine we're coding in c+ and linux using .svg as the modle texture to maximum fps\n"),
        ]),
        UserMessage(content = [
            TextContentItem(text = "Update or game egine so we can test the game\n"),
        ]),
    ],
    model = "gpt-4o",
    tools = [],
    response_format = "text",
    temperature = 1,
    top_p = 1,
)

if response.choices[0].message.tool_calls:
    print(response.choices[0].message.tool_calls)
else:
    print(response.choices[0].message.content)
