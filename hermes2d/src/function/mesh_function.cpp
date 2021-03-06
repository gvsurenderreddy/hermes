// This file is part of Hermes2D.
//
// Hermes2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Hermes2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hermes2D.  If not, see <http://www.gnu.org/licenses/>.

#include "solution.h"

namespace Hermes
{
  namespace Hermes2D
  {
    template<typename Scalar>
    MeshFunctionSharedPtr<Scalar>::MeshFunctionSharedPtr(Hermes::Hermes2D::MeshFunction<Scalar> * ptr) : std::tr1::shared_ptr<Hermes::Hermes2D::MeshFunction<Scalar> >(ptr)
    {
    }

    template<typename Scalar>
    MeshFunctionSharedPtr<Scalar>::MeshFunctionSharedPtr(const MeshFunctionSharedPtr& other) : std::tr1::shared_ptr<Hermes::Hermes2D::MeshFunction<Scalar> >(other)
    {
    }

    template<typename Scalar>
    void MeshFunctionSharedPtr<Scalar>::operator=(const MeshFunctionSharedPtr& other)
    {
      std::tr1::shared_ptr<Hermes::Hermes2D::MeshFunction<Scalar> >::operator=(other);
    }

    template<typename Scalar>
    Hermes::Hermes2D::Solution<Scalar>* MeshFunctionSharedPtr<Scalar>::get_solution()
    {
      return dynamic_cast<Hermes::Hermes2D::Solution<Scalar>*>(this->get());
    }

    template class HERMES_API MeshFunctionSharedPtr < double > ;
    template class HERMES_API MeshFunctionSharedPtr < std::complex<double> > ;

    template<typename Scalar>
    MeshFunction<Scalar>::MeshFunction()
      : Function<Scalar>()
    {
      this->element = nullptr;
    }

    template<typename Scalar>
    MeshFunction<Scalar>::MeshFunction(MeshSharedPtr mesh) :
      Function<Scalar>()
    {
      this->mesh = mesh;
    }

    template<typename Scalar>
    MeshFunction<Scalar>::~MeshFunction()
    {
      free();
    }

    template<typename Scalar>
    std::string MeshFunction<Scalar>::getClassName() const
    {
      return "MeshFunction";
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::copy(const MeshFunction<Scalar>* sln)
    {
      throw Exceptions::Exception("This instance is in fact not a Solution instance in copy().");
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::copy(MeshFunctionSharedPtr<Scalar> sln)
    {
      copy(sln.get());
    }

    template<typename Scalar>
    bool MeshFunction<Scalar>::isOkay() const
    {
      bool okay = true;
      if (this->mesh == nullptr)
        okay = false;
      try
      {
        if (this->mesh->get_max_element_id() < 0)
          throw Hermes::Exceptions::Exception("Internal exception.");
        this->mesh->get_element(this->mesh->get_max_element_id() - 1);
      }
      catch (std::exception& e)
      {
        std::cout << e.what();
        okay = false;
      }
      return okay;
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::free()
    {
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::init()
    {
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::reinit()
    {
      this->free();
      init();
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::add(MeshFunctionSharedPtr<Scalar>& other_mesh_function, SpaceSharedPtr<Scalar> target_space)
    {
      throw Exceptions::MethodNotOverridenException("MeshFunction<Scalar>::add");
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::multiply(Scalar coef)
    {
      throw Exceptions::MethodNotOverridenException("MeshFunction<Scalar>::multiply");
    }

    template<>
    double MeshFunction<double>::get_approx_max_value(int item_)
    {
      this->check();

      Quad2D *old_quad = this->get_quad_2d();
      this->set_quad_2d(&g_quad_lin);

      double max = std::numeric_limits<double>::min();

      int component = 0;
      int value_type = 0;
      int item = item_;

      if (item >= 0x40)
      {
        component = 1;
        item >>= 6;
      }
      while (!(item & 1))
      {
        item >>= 1;
        value_type++;
      }

      item = item_;

      Element* e;
      for_all_active_elements(e, this->mesh)
      {
        this->set_active_element(e);
        this->set_quad_order(1, item);
        const double* val = this->get_values(component, value_type);
        for (int i = 0; i < (e->is_triangle() ? 3 : 4); i++)
        {
          double v = val[i];
          if (v > max)
            max = v;
        }
      }

      this->set_quad_2d(old_quad);
      return max;
    }

    template<>
    std::complex<double> MeshFunction<std::complex<double> >::get_approx_max_value(int item_)
    {
      this->check();
      this->warn("Asked for a max value of a complex function.");
      return std::numeric_limits<std::complex<double> >::min();
    }

    template<>
    double MeshFunction<double>::get_approx_min_value(int item_)
    {
      this->check();

      Quad2D *old_quad = this->get_quad_2d();
      this->set_quad_2d(&g_quad_lin);

      double min = std::numeric_limits<double>::max();

      int component = 0;
      int value_type = 0;
      int item = item_;

      if (item >= 0x40)
      {
        component = 1;
        item >>= 6;
      }
      while (!(item & 1))
      {
        item >>= 1;
        value_type++;
      }

      item = item_;

      Element* e;
      for_all_active_elements(e, this->mesh)
      {
        this->set_active_element(e);
        this->set_quad_order(1, item);
        const double* val = this->get_values(component, value_type);
        for (int i = 0; i < (e->is_triangle() ? 3 : 4); i++)
        {
          double v = val[i];
          if (v < min)
            min = v;
        }
      }

      this->set_quad_2d(old_quad);
      return min;
    }

    template<>
    std::complex<double> MeshFunction<std::complex<double> >::get_approx_min_value(int item_)
    {
      this->check();
      this->warn("Asked for a min value of a complex function.");
      return std::numeric_limits<std::complex<double> >::max();
    }

    template<typename Scalar>
    int MeshFunction<Scalar>::get_edge_fn_order(int edge)
    {
      return Function<Scalar>::get_edge_fn_order(edge);
    }

    template<typename Scalar>
    MeshSharedPtr MeshFunction<Scalar>::get_mesh() const
    {
      return mesh;
    }

    template<typename Scalar>
    RefMap* MeshFunction<Scalar>::get_refmap(bool update)
    {
      if (update)
        this->update_refmap();
      return &refmap;
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::set_quad_2d(Quad2D* quad_2d)
    {
      if (quad_2d == nullptr)
        throw Exceptions::NullException(1);
      Function<Scalar>::set_quad_2d(quad_2d);
      refmap.set_quad_2d(quad_2d);
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::set_active_element(Element* e)
    {
      if (e && !e->active)
        throw Hermes::Exceptions::Exception("Cannot select inactive element. Wrong mesh?");

      Function<Scalar>::set_active_element(e);
      mode = e->get_mode();
      refmap.set_active_element(e);
    }

    template<typename Scalar>
    void MeshFunction<Scalar>::update_refmap()
    {
      refmap.force_transform(this->sub_idx, this->ctm);
    }

    template class HERMES_API MeshFunction < double > ;
    template class HERMES_API MeshFunction < std::complex<double> > ;
  }
}