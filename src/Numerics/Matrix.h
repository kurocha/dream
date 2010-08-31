/*
 *  Numerics/Matrix.h
 *  This file is part of the "Dream" project, and is licensed under the GNU GPLv3.
 *
 *  Created by Samuel Williams on 13/03/06.
 *  Copyright 2006 Samuel Williams. All rights reserved.
 *
 */

#ifndef _DREAM_NUMERICS_MATRIX_H
#define _DREAM_NUMERICS_MATRIX_H

#include "Numerics.h"

#include "Vector.h"
#include <algorithm>

namespace Dream
{
	namespace Numerics
	{
		template <unsigned, unsigned, typename>
		class Matrix;

#pragma mark -
#pragma mark Matrix Multiplication Traits

		template <unsigned R, unsigned C, typename NumericT>
		class MatrixMultiplicationTraits
		{
		private:
			typedef Matrix<R, C, NumericT> MatrixT;

		public:
			Vector<C, NumericT> multiply (const Vector<R, NumericT> & v) const;

			template <unsigned T>
			Matrix<T, C, NumericT> multiply (const Matrix<T, R, NumericT> & m) const;
			
			/// Short-hand notation
			Vector<C, NumericT> operator* (const Vector<R, NumericT> & v) const
			{
				return this->multiply(v);
			}
			
			/// Short-hand notation for non-homogeneous vectors
			Vector<C-1, NumericT> operator* (const Vector<R-1, NumericT> & v) const
			{
				Vector<C, NumericT> result (this->multiply(v << 1));
				
				result /= result[C-1];
				
				return result.reduce();
			}
			
			/// Short hand for matrix multiplication
			template <unsigned T>
			Matrix<T, C, NumericT> operator* (const Matrix<T, R, NumericT> & other) const
			{
				return this->multiply(other);
			}
		};

#pragma mark -
#pragma mark Matrix Inverse Traits

		template <unsigned R, unsigned C, typename NumericT>
		class MatrixInverseTraits
		{
		};

		template <typename NumericT>
		class MatrixInverseTraits<4, 4, NumericT>
		{
		private:
			typedef Matrix<4, 4, NumericT> MatrixT;

			inline const NumericT * values () const;

		public:
			Matrix<4, 4, NumericT> inverseMatrix () const;
		};

#pragma mark -
#pragma mark Matrix Square Traits

		template <unsigned R, unsigned C, typename NumericT>
		class MatrixSquareTraits
		{};

		template <unsigned N, typename NumericT>
		class MatrixSquareTraits<N, N, NumericT>
		{
		private:
			typedef Matrix<N, N, NumericT> MatrixT;

		public:
			template <unsigned K>
			static MatrixT scalingMatrix (const Vector<K, NumericT> & amount);

			template <unsigned K>
			static MatrixT translatingMatrix (const Vector<K, NumericT> & amount);

			// Rotations in 3D.
			static MatrixT rotatingMatrix (const NumericT & radians, const Vector<3, NumericT> & normal);
			static MatrixT rotatingMatrix (const NumericT & radians, const Vector<3, NumericT> & normal, const Vector<3, NumericT> & point);
			static MatrixT rotatingMatrix (const Vector<3, NumericT> & from, const Vector<3, NumericT> & to, const Vector<3, NumericT> & normal);

			// Rotations in 2D.
			static MatrixT rotatingMatrixAroundX (const NumericT & radians);
			static MatrixT rotatingMatrixAroundY (const NumericT & radians);
			/// Also works for Matrix<2,2>
			static MatrixT rotatingMatrixAroundZ (const NumericT & radians);

			// Convenience Functions
			MatrixT rotatedMatrix (const NumericT & radians, const Vector<3, NumericT> & normal);
			MatrixT rotatedMatrix (const NumericT & radians, const Vector<3, NumericT> & normal, const Vector<3, NumericT> & point);

			template <unsigned K>
			MatrixT scaledMatrix (const Vector<K, NumericT> & amount);

			template <unsigned K>
			MatrixT translatedMatrix (const Vector<K, NumericT> & amount);

			/// In-place transposition (transposeMatrix)
			MatrixT & transpose ();
		};

#pragma mark -

		template <unsigned R, unsigned C, typename NumericT>
		class MatrixEqualityTraits
		{
		};

		template <unsigned R, unsigned C>
		class MatrixEqualityTraits<R, C, float>
		{
		protected:
			typedef Matrix<R, C, float> MatrixT;

		public:
			bool equalWithinTolerance (const MatrixT & other, const unsigned & ulps = DEFAULT_ULPS) const;
		};

		template <unsigned R, unsigned C>
		class MatrixEqualityTraits<R, C, double>
		{
		protected:
			typedef Matrix<R, C, double> MatrixT;

		public:
			bool equalWithinTolerance (const MatrixT & other, const unsigned & ulps = DEFAULT_ULPS) const;
		};

#pragma mark -
#pragma mark Matrix Class

		unsigned rowMajorOffset(unsigned row, unsigned col, unsigned sz);
		unsigned columnMajorOffset(unsigned row, unsigned col, unsigned sz);

		/** A 2-dimentional set of numbers that can represent useful transformations in n-space.
		
		Standard mathematical notation is column order, therefore regardless of row-major or column-major memory layout,
		the interface will assume access is done via rows and columns according to this standard notation.
		 */
		template <unsigned _R = 4, unsigned _C = 4, typename _NumericT = RealT>
		class Matrix : public MatrixSquareTraits<_R, _C, _NumericT>, public MatrixMultiplicationTraits<_R, _C, _NumericT>,
					   public MatrixInverseTraits<_R, _C, _NumericT>, public MatrixEqualityTraits<_R, _C, _NumericT>
		{
		public:
			enum { R = _R };
			enum { C = _C };
			typedef _NumericT NumericT;

		protected:
			NumericT m_matrix[R*C];
		
		public:
			// Uninitialized constructor
			Matrix ();
			Matrix (const Zero &);
			Matrix (const Identity &);

			Matrix (const Matrix<R, C, NumericT> & other);
			
			Matrix (const NumericT * data)
			{ 
				set(data);
			}

			void set (const NumericT * data)
			{
				memcpy(m_matrix, data, sizeof(m_matrix));
			}
			
			template <typename AnyT>
			void set (const AnyT * data)
			{
				for (unsigned i = 0; i < R*C; i++)
				{
					m_matrix[i] = data[i];
				}
			}
			
			void zero ();
			void loadIdentity (const NumericT & n = 1);

			// Accessors
			const NumericT & at (unsigned r, unsigned c) const
			{
				ensure(rowMajorOffset(r, c, C) < R*C);
				return m_matrix[rowMajorOffset(r, c, C)];
			}

			NumericT & at (unsigned r, unsigned c)
			{
				ensure(rowMajorOffset(r, c, C) < R*C);
				return m_matrix[rowMajorOffset(r, c, C)];
			}
			
			const NumericT & at (unsigned i) const
			{
				ensure(i < R*C);
				return m_matrix[i];
			}
			
			NumericT & at (unsigned i)
			{
				ensure(i < R*C);
				return m_matrix[i];
			}

			NumericT * value ()
			{
				return (NumericT*)m_matrix;
			}

			const NumericT * value () const
			{
				return (const NumericT*)m_matrix;
			}
			
			/// Copy a vector into the matix at position r, c
			/// This copies the vector in the direction of the major format,
			/// i.e. in column major format it will appear as a column
			template <unsigned D>
			void set (const IndexT & r, const IndexT & c, const Vector<D, NumericT> & v)
			{
				memcpy(&at(r, c), v.value(), sizeof(NumericT) * D);
			}
			
			/// Copy a vector into the matrix at position r, c, with elementOffset distance between each element.
			/// The purpose of this function is primarily to facilitate copying a vector into a matrix in an order
			/// other than the major.
			/// i.e. set(0, 0, Vec4(...), 4) will set a row in a column major matrix.
			template <unsigned D>
			void set (const IndexT & r, const IndexT & c, const Vector<D, NumericT> & v, IndexT elementOffset)
			{
				IndexT offset = &at(r, c) - (NumericT*)m_matrix;
								
				for (IndexT i = 0; i < D; i += 1) {
					value()[offset + elementOffset * i] = v[i];
				}
			}
			
			/// @todo Write get equivalent of set functions for retriving Vector data
			
			/// Return a copy of this matrix, transposed.
			Matrix<C, R, NumericT> transposedMatrix () const
			{
				Matrix<C, R, NumericT> result;

				for (unsigned c = 0; c < C; ++c)
					for (unsigned r = 0; r < R; ++r)
						result.at(c, r) = at(r, c);

				return result;
			}
			
			/// Load a test patern into the matrix. Used for testing.
			void loadTestPattern ()
			{
				unsigned i = 0;

				for (unsigned r = 0; r < R; r += 1)
					for (unsigned c = 0; c < C; c += 1)
						at(r, c) = i++;
			}
			
			/// Check if a matrices components are exactly equal.
			bool operator== (const Matrix & other) const
			{
				for (unsigned c = 0; c < C; ++c)
					for (unsigned r = 0; r < R; ++r)
						if (at(r, c) != other.at(r, c))
							return false;

				return true;
			}
		};

#pragma mark -
#pragma mark Static Matrix Constructors

		/// Convenience type for matrix class
		typedef Matrix<4, 4, RealT> Mat44;
		/// Convenience type for matrix class
		typedef Matrix<3, 3, RealT> Mat33;
		/// Convenience type for matrix class
		typedef Matrix<2, 2, RealT> Mat22;

		/// Convenience constructor for matrix class
		Mat44 rotation (const RealT & radians, const Vec3 & aroundNormal, const Vec3 & aroundPoint);
		/// Convenience constructor for matrix class
		Mat44 rotation (const RealT & radians, const Vec3 & aroundNormal);
		/// Convenience constructor for matrix class
		Mat44 rotation (const Vec3 & fromUnitVector, const Vec3 & toUnitVector, const Vec3 & aroundNormal);

	}
}

#include "Matrix.impl.h"

#endif