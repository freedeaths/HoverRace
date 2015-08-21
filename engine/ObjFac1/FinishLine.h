
// FinishLine.h
//
// Copyright (c) 1995-1998 - Richard Langlois and Grokksoft Inc.
//
// Licensed under GrokkSoft HoverRace SourceCode License v1.0(the "License");
// you may not use this file except in compliance with the License.
//
// A copy of the license should have been attached to the package from which
// you have taken this file. If you can not find the license you can not use
// this file.
//
//
// The author makes no representations about the suitability of
// this software for any purpose.  It is provided "as is" "AS IS",
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied.
//
// See the License for the specific language governing permissions
// and limitations under the License.
//

#pragma once

#include "../Model/MazeElement.h"
#include "../Model/RaceEffects.h"

namespace HoverRace {
namespace ObjFac1 {

/**
 * @note Finish lines are not finish lines; they are checkpoints.
 */
class FinishLine :
	public Model::FreeElement,
	protected Model::CylinderShape
{
	using SUPER = Model::FreeElement;

public:
	FinishLine(Model::CheckPoint::CheckPointType pType);
	~FinishLine() { }

protected:
	// Shape interface
	MR_Int32 ZMin() const override;
	MR_Int32 ZMax() const override;
	MR_Int32 AxisX() const override;
	MR_Int32 AxisY() const override;
	MR_Int32 RayLen() const override;

protected:
	// ContactEffectShapeInterface
	const Model::ContactEffectList *GetEffectList() override { return &mContactEffectList; }
	const Model::ShapeInterface *GetReceivingContactEffectShape() override { return this; }

private:
	Model::CheckPoint mEffect;
	Model::ContactEffectList mContactEffectList;
};

}  // namespace ObjFac1
}  // namespace HoverRace
