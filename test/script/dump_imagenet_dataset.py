import argparse
import os
import torch

from torchvision import datasets, transforms
from tqdm import tqdm

from utils import write_tensor_to_file
from datasets import load_dataset


def get_transforms(model_type):
    if model_type == "resnet":
        return transforms.Compose(
            [
                transforms.Resize(256),
                transforms.CenterCrop(224),
                transforms.ToTensor(),
                transforms.Lambda(
                    lambda x: x.repeat(3, 1, 1) if x.shape[0] == 1 else x
                ),  # Convert grayscale to 3-channel
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


def dump_imagenet(output_dir, num_samples, model_type):
    transform = get_transforms(model_type)

    # Load the ImageNet dataset from Hugging Face
    dataset = load_dataset("timm/imagenet-1k-wds", split="validation", streaming=True)
    dataset = dataset.take(num_samples)

    # Iterate over the selected indices and retrieve samples
    for i, item in tqdm(enumerate(dataset), desc="Dumping dataset"):
        image = transform(item["jpg"])
        label = item["cls"]

        dir_name = os.path.join(output_dir, f"{i}_{label}")
        os.makedirs(dir_name, exist_ok=True)
        image = image.permute(1, 2, 0)  # permute to HWC format

        filename = (
            "x_preprocess.bin"
            if model_type == "resnet"
            else "pixel_values_preprocess.bin"
        )

        write_tensor_to_file(image, os.path.join(dir_name, filename))


def main():
    torch.set_num_threads(32)

    parser = argparse.ArgumentParser(
        description="ResNet data generator (from model and images to binary data)."
    )

    parser.add_argument(
        "--output_dir",
        default="data/imagenet",
        help="Path to output folder for dataset.",
    )
    parser.add_argument(
        "--num_samples",
        type=int,
        default=None,
        help="How many samples to generate.",
    )
    parser.add_argument(
        "--model_type",
        type=str,
        choices=["resnet", "vit"],
        help="Model type to use for preprocessing.",
    )
    args = parser.parse_args()
    dump_imagenet(args.output_dir, args.num_samples, args.model_type)


if __name__ == "__main__":
    main()
