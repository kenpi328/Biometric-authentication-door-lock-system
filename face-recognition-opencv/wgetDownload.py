import wget
img_url = "file:///home/ken/Pictures/test_face_recognition/known_people/ken.jpg"
local_image_filename = wget.download(img_url)
