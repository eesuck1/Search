import sys
import os

import pypdf


def read_pdf(source_path: str, destination_path: str) -> None:
    _, source_extension = os.path.splitext(source_path)
    _, destination_extension = os.path.splitext(destination_path)

    if source_extension != ".pdf":
        raise Exception(f"Incorrect Source File Path Extension! (expected pdf got {source_extension})")

    if destination_extension != ".txt":
        raise Exception(f"Incorrect Destination File Path Extension! (expected txt got {destination_extension})")

    reader = pypdf.PdfReader(source_path)

    with open(destination_path, "a", encoding="utf-8") as file:
        for page in reader.pages:
            file.write(page.extract_text() + "\n")


if __name__ == '__main__':
    if len(sys.argv) < 3:
        raise Exception(f"Incorrect usage!\nUse: interpreter parser.py source.pdf destination.txt")
    
    try:
        read_pdf(sys.argv[1], sys.argv[2])
    except Exception as error:
        print(f"[PY ERROR] [{error}] Could not read - {sys.argv[1]}")

