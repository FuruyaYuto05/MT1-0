#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "学籍番号";
const int kRowHeight = 20;

// Vector3構造体
struct Vector3 {
	float x;
	float y;
	float z;
};

// Matrix4x4構造体
struct Matrix4x4 {
	float m[4][4];
};

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 matrix = {};
	float tanHalfFovY = tanf(fovY / 2.0f);

	// 透視投影行列の計算
	matrix.m[0][0] = 1.0f / (aspectRatio * tanHalfFovY);
	matrix.m[1][1] = 1.0f / tanHalfFovY;
	matrix.m[2][2] = -(farClip + nearClip) / (farClip - nearClip);
	matrix.m[2][3] = -1.0f;
	matrix.m[3][2] = -(2.0f * farClip * nearClip) / (farClip - nearClip);
	matrix.m[3][3] = 0.0f;

	return matrix;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 matrix = {};

	// 正射影行列の計算
	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = -2.0f / (farClip - nearClip);
	matrix.m[3][0] = -(right + left) / (right - left);
	matrix.m[3][1] = -(top + bottom) / (top - bottom);
	matrix.m[3][2] = -(farClip + nearClip) / (farClip - nearClip);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 matrix = {};

	// ビューポート行列の計算
	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -height / 2.0f;
	matrix.m[2][2] = maxDepth - minDepth;
	matrix.m[3][0] = left + width / 2.0f;
	matrix.m[3][1] = top + height / 2.0f;
	matrix.m[3][2] = minDepth;

	// 位置と拡大縮小を調整するための値
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// 行列表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; row++) {
		Novice::ScreenPrintf(x, y + (row + 1) * kRowHeight,
			"%.2f  %.2f  %.2f  %.2f",
			matrix.m[row][0], matrix.m[row][1], matrix.m[row][2], matrix.m[row][3]);
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// 更新処理
		Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);
		Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

		// 描画処理
		MatrixScreenPrintf(0, 0, orthographicMatrix, "orthographicMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, perspectiveFovMatrix, "perspectiveFovMatrix");
		MatrixScreenPrintf(0, kRowHeight * 10, viewportMatrix, "viewportMatrix");

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
