package GUI

import (
	_ "image/jpeg"
	_ "image/png"
)

var pyscript = `
from skimage.metrics import structural_similarity as ssim
from skimage.io import imread

a=imread("%s")
b=imread("%s")
print(ssim(a,b,multichannal=True))
`

func image_from_coordinates() {}
func unlockByPicture()        {}

/*
func image_read(fname string) (*image.RGBA, error) {
	f, err := os.Open(fname)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	im, _, err := image.Decode(f)
	rgba := image.NewRGBA(im.Bounds())
	draw.Copy(rgba, image.Point{}, im, im.Bounds(), draw.Src, nil)
	return rgba, nil
}

//export ssim_verify
func ssim_verify(a, b string) bool {
	c := bytes.NewBuffer([]byte{})
	f, _ := os.CreateTemp("", "ssim")
	defer f.Close()
	cmd := exec.Command("python", f.Name())
	cmd.Stdout = c
	cmd.Run()
	i, err := strconv.ParseFloat(string(c.Bytes()), 64)
	if err != nil {
		return false
	}
	return i > 0.9
}
*/
