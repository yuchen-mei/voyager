import argparse
import os
import torch
from torchvision import transforms
from tqdm import tqdm
from transformers import ViTForImageClassification
from datasets import load_dataset


def get_transforms(model_type):
    if model_type == "resnet":
        return transforms.Compose(
            [
                transforms.Resize(256),
                transforms.CenterCrop(224),
                transforms.ToTensor(),
                transforms.Normalize(
                    mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]
                ),
            ]
        )
    elif model_type == "vit":
        return transforms.Compose(
            [
                transforms.Resize(
                    256, interpolation=transforms.InterpolationMode.BICUBIC
                ),
                transforms.CenterCrop(224),
                transforms.ToTensor(),
                transforms.Lambda(
                    lambda x: x.repeat(3, 1, 1) if x.shape[0] == 1 else x
                ),  # Convert grayscale to 3-channel
                transforms.Normalize(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5]),
            ]
        )
    else:
        raise ValueError(f"Unsupported model type: {model_type}")


def evaluate_vit_on_imagenet(num_samples=1000):
    print("Starting evaluation on ImageNet validation set from Hugging Face datasets")

    vit_transforms = get_transforms("vit")

    # Load the ImageNet dataset from Hugging Face
    dataset = load_dataset("timm/imagenet-1k-wds", split="validation", streaming=True)
    dataset = dataset.take(num_samples)

    model = ViTForImageClassification.from_pretrained("google/vit-base-patch16-224")
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.to(device)
    model.eval()

    correct_predictions = 0
    total_samples = 0

    print("Starting evaluation loop...")
    with torch.no_grad():
        for item in tqdm(dataset, desc="Evaluating"):
            # Convert PIL image to tensor and apply transforms
            image = vit_transforms(item["jpg"])
            image = image.unsqueeze(0).to(device)  # Add batch dimension

            # Get the label
            label = item["cls"]

            # Get model prediction
            outputs = model(image)
            logits = outputs.logits
            prediction = torch.argmax(logits, dim=-1)

            # Update accuracy
            if prediction.item() == label:
                correct_predictions += 1
            total_samples += 1

    accuracy = correct_predictions / total_samples if total_samples > 0 else 0.0
    print(f"Accuracy on the ImageNet validation subset: {accuracy:.4f}")
    return accuracy


def main():
    torch.set_num_threads(32)
    parser = argparse.ArgumentParser(description="Evaluate ViT on ImageNet.")
    parser.add_argument(
        "--num_samples", type=int, default=1000, help="Number of samples to evaluate."
    )
    args = parser.parse_args()
    accuracy = evaluate_vit_on_imagenet(args.num_samples)
    if accuracy is not None:
        print(f"Final Evaluated Accuracy: {accuracy:.4f}")


if __name__ == "__main__":
    main()
