package GUI

import (
	"image"
	_ "image/jpeg"
	_ "image/png"
	"os"

	"golang.org/x/image/draw"
)

var pyscript = `
from skimage.metrics import structural_similarity as ssim

`

func image_from_coordinates() {}
func unlockByPicture()        {}

func image_read(fname string) ([]uint8, error) {
	f, err := os.Open(fname)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	im, _, err := image.Decode(f)
	rgba := image.NewRGBA(im.Bounds())
	draw.Copy(rgba, image.Point{}, im, im.Bounds(), draw.Src, nil)
	return rgba.Pix, nil
}

func structural_similarity(im1, im2 []uint8) float64 {}
